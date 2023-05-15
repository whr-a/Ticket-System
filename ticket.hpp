#ifndef TICKET
#define TICKET
#include "trains.hpp"
#include "users.hpp"
#include "utility.hpp"
enum status{success,pending,refunded};
class order
{
public:
    int time;//时间戳
    int date;//第几天的车
    status status;
    char trainID[21];
    char from[33];
    int no1;//起始第几站
    char to[33];
    int no2;//结束第几站
    int price;
    int num;
    Time leaving,arriving;
    order(){}
    order(int time_,int date_,enum status status_,char* trainID_,char* from_,int no1_,
                char* to_,int no2_,int price_,int num_,Time leaving_,Time arriving_):
                time(time_),date(date_),status(status_),no1(no1_),no2(no2_),price(price_),num(num_),leaving(leaving_),arriving(arriving_){
        strcpy(trainID,trainID_);
        strcpy(from,from_);
        strcpy(to,to_);
    }
    friend bool operator< (const order &obj1,const order &obj2){
        return obj1.time>obj2.time;
    }
};
class train_f
{
public:
    char trainID[21];
    int date;//第几天的车
    train_f(){}
    train_f(char* trainID_,int date_):date(date_){
        strcpy(trainID,trainID_);
    }
    friend bool operator<(const train_f &obj1,const train_f &obj2){
        int t=strcmp(obj1.trainID,obj2.trainID);
        if(t<0)return true;
        else if(t==0 && obj1.date<obj2.date)return true;
        else return false;
    }
};
class candidate
{
public:
    int time;//时间戳
    char u[21];
    int no1;
    int no2;
    int n;
    candidate(){}
    candidate(int time_,char* u_,int no1_,int no2_,int n_):time(time_),no1(no1_),no2(no2_),n(n_){
        strcpy(u,u_);
    }
    friend bool operator<(const candidate &obj1,const candidate &obj2){
        return obj1.time<obj2.time;
    }
};
class ticket
{
public:
    database<username,order> order_base;
    database<train_f,candidate> candidate_base;
    ticket(){
        order_base.setfile("order.db");
        candidate_base.setfile("candidate.db");
    }
    int buy_ticket(char* u,char* i,int mm,int dd,int n,char* f,char* t,bool q,train trains,user users,int times){
        if(users.login_set.find(username(u))==users.login_set.end())return -1;
        sjtu::vector<train_inf> result=trains.train_base.find(trainID(i));
        if(!result[0].release)return -1;
        int no1,no2;
        for(no1=0;no1<result[0].stationNum;no1++)if(strcmp(f,result[0].stations[no1])==0)break;
        for(no2=0;no2<result[0].stationNum;no2++)if(strcmp(t,result[0].stations[no2])==0)break;
        Time x(7,15,result[0].startTime.hh,result[0].startTime.mm);
        x=x+result[0].time_sum[no1];
        x.setMonth(mm);x.setDay(dd);
        x=x-result[0].time_sum[no1];
        Time st(result[0].saleDate[0],result[0].startTime),
            ed(result[0].saleDate[1],result[0].startTime);
        if(x<st || x>ed)return -1;
        x=x+result[0].time_sum[no1];
        int day_=x.days_to(Time(result[0].saleDate[0]));
        int seats=200000;
        for(int k=no1;k<no2;k++){
            seats=std::min(seats,result[0].ticket_left[day_][k]);
        }
        if(seats>=n){
            //更新余位
            trains.train_base.erase(trainID(i),result[0]);
            for(int k=no1;k<no2;k++)result[0].ticket_left[day_][k]-=n;
            trains.train_base.insert(trainID(i),result[0]);
            //记入用户的订单信息
            int money_s=result[0].money_sum[no2]-result[0].money_sum[no1];
            Time ar=x+(result[0].time_sum[no2]-result[0].time_sum[no1]-result[0].stopoverTimes[no2-1]);
            order_base.insert(username(u),order(times,day_,success,i,f,no1,t,no2,
                    money_s,n,x,ar));
            return money_s;
        }
        else{
            //记入用户的订单信息
            int money_s=result[0].money_sum[no2]-result[0].money_sum[no1];
            Time ar=x+(result[0].time_sum[no2]-result[0].time_sum[no1]-result[0].stopoverTimes[no2-1]);
            order_base.insert(username(u),order(times,day_,pending,i,f,no1,t,no2,
                    money_s,n,x,ar));
            //加入候补队列
            candidate_base.insert(train_f(i,day_),candidate(times,u,no1,no2,n));
            return -2;
        }
    }
    int query_order(char* u,train trains,user users){
        if(users.login_set.find(username(u))==users.login_set.end())return -1;
        sjtu::vector<order> result=order_base.find(username(u));
        std::cout<<result.size();
        for(int i=0;i<result.size();i++){
            if(result[i].status==success)std::cout<<"[success] ";
            else if(result[i].status==pending)std::cout<<"[pending] ";
            else std::cout<<"[refunded] ";
            std::cout<<result[i].trainID<<' '<<result[i].from<<' '<<result[i].leaving.toString()<<" -> "
                     <<result[i].to<<' '<<result[i].arriving.toString()<<' '<<result[i].price<<' '<<result[i].num<<'\n'; 
        }
        return 1;
    }
    int refund_ticket(char* u,int n,train trains,user users){
        if(users.login_set.find(username(u))==users.login_set.end())return -1;
        sjtu::vector<order> result=order_base.find(username(u));
        if(result.size()<n || result[n-1].status==refunded)return -1;
        if(result[n-1].status==pending){
            order_base.erase(username(u),result[n-1]);
            result[n-1].status=refunded;
            order_base.insert(username(u),result[n-1]);
            //移出候补队列
            candidate_base.erase(train_f(result[n-1].trainID,result[n-1].date),
                candidate(result[n-1].time,u,result[n-1].no1,result[n-1].no2,result[n-1].num));//从候补列表删除
        }
        else{
            //订单状态更新
            order_base.erase(username(u),result[n-1]);
            result[n-1].status=refunded;
            order_base.insert(username(u),result[n-1]);
            //火车余票更新
            sjtu::vector<train_inf> results=trains.train_base.find(result[n-1].trainID);
            trains.train_base.erase(result[n-1].trainID,results[0]);
            for(int i=result[n-1].no1;i<result[n-1].no2;i++)
                results[0].ticket_left[result[n-1].date][i]+=result[n-1].num;
            //候补尝试
            sjtu::vector<candidate> waiting=
                candidate_base.find(train_f(result[n-1].trainID,result[n-1].date));
            for(int i=0;i<waiting.size();i++){
                int seats_left=200000;
                for(int j=waiting[i].no1;j<waiting[i].no2;j++)
                    seats_left=std::min(seats_left,results[0].ticket_left[result[n-1].date][j]);
                if(seats_left>=waiting[i].n){
                    for(int j=waiting[i].no1;j<waiting[i].no2;j++)
                        results[0].ticket_left[result[n-1].date][j]-=waiting[i].n;
                    sjtu::vector<order> orders=order_base.find(waiting[i].u);
                    int l=0,r=orders.size()-1,mid;//找到时间戳为waiting[i].time,在第mid位
                    while(l<=r){
                        mid=(l+r)>>1;
                        if(waiting[i].time==orders[mid].time)break;
                        if(waiting[i].time<orders[mid].time)l=mid+1;
                        else r=mid-1;
                    }
                    order_base.erase(waiting[i].u,orders[mid]);
                    orders[mid].status=success;
                    order_base.insert(waiting[i].u,orders[mid]);
                    candidate_base.erase(train_f(result[n-1].trainID,result[n-1].date),waiting[i]);
                }
            }
            //最后记得把results[0]写回去
            trains.train_base.insert(result[n-1].trainID,results[0]);
        }
        return 0;
    }
};


#endif