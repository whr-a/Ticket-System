#ifndef TICKET
#define TICKET
#include "trains.hpp"
#include "users.hpp"
#include "ticket_left.hpp"
#include "utility.hpp"
enum status{success,pending,refunded};
class order
{
public:
    int time;//时间戳
    int date;//第几天的车
    status status_;
    char trainID[21];
    char from[33];
    int no1;//起始第几站
    char to[33];
    int no2;//结束第几站
    int price;
    int num;
    Time leaving,arriving;
    order(){time=0;trainID[0]=0;from[0]=0;to[0]=0;}
    order(int time_,int date_,status status__,char* trainID_,char* from_,int no1_,
                char* to_,int no2_,int price_,int num_,Time leaving_,Time arriving_):
                time(time_),date(date_),status_(status__),no1(no1_),no2(no2_),price(price_),num(num_),leaving(leaving_),arriving(arriving_){
        strcpy(trainID,trainID_);
        strcpy(from,from_);
        strcpy(to,to_);
    }
    friend bool operator< (const order &obj1,const order &obj2){
        return obj1.time>obj2.time;
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
    candidate(){time=0;}
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
        order_base.setfile("order.db");//设置文件名称
        candidate_base.setfile("candidate.db");
    }
    void clear(){
        order_base.clear("order.db");//清除文件
        candidate_base.clear("candidate.db");
    }
    int buy_ticket(char* u,char* i,int mm,int dd,int n,char* f,char* t,bool q,train &trains,user &users,int times,ticket_base &ticket_base_){
        if(users.login_set.find(username(u))==users.login_set.end()){
            // std::cout<<444<<std::endl;
            return -1;//用户没登录不能买票
        }
        sjtu::vector<train_inf> result=trains.train_base.find(trainID(i));//找到火车信息，存在result[0]里
        // std::cout<<"**************\n";
        // for(int i=0;i<result[0].stationNum;i++)std::cout<<result[0].money_sum[i]<<' ';
        // std::cout<<"**************"<<std::endl;
        if(!result[0].release){
            // std::cout<<111<<std::endl;
            return -1;//没放票不能买票
        }
        
        int no1,no2;//找到这两站分别是哪一站
        for(no1=0;no1<result[0].stationNum;no1++)if(strcmp(f,result[0].stations[no1])==0)break;
        for(no2=0;no2<result[0].stationNum;no2++)if(strcmp(t,result[0].stations[no2])==0)break;
        if(no1==result[0].stationNum || no2==result[0].stationNum || no1>=no2){
            // std::cout<<222<<std::endl;
            // std::cout<<no1<<' '<<no2<<std::endl;
            return -1;
        }
        Time x(7,15,result[0].startTime.hh,result[0].startTime.mm);//开始时间固定
        x=x+result[0].time_sum[no1];//得到到始发站的时间
        x.setMonth(mm);x.setDay(dd);//再把月份和日给换一下
        x=x-result[0].time_sum[no1];//减回去得到始发站发车的时间
        Time st(result[0].saleDate[0],result[0].startTime),
            ed(result[0].saleDate[1],result[0].startTime);
        if(x<st || x>ed){
            // std::cout<<st.toString()<<' '<<ed.toString()<<std::endl;
            // std::cout<<x.toString()<<std::endl;
            return -1;//判断发车时间是否合法
        }
        int day_=x.days_to(Time(result[0].saleDate[0]));//得到发车时间是第几天
        x=x+result[0].time_sum[no1];//得到从出发站的发车时间
        int seats=200000;
        ticket_left left_temp(ticket_base_.query(train_f(i,day_)));//调出第day_天发车的余票数组
        for(int k=no1;k<no2;k++){
            seats=std::min(seats,left_temp.ticket[k]);//找到seats
        }
        if(seats>=n){
            //更新余位
            for(int k=no1;k<no2;k++)left_temp.ticket[k]-=n;//从no1到no2-1的所有票数减n
            ticket_base_.modify(train_f(i,day_),left_temp);
            //记入用户的订单信息
            int money_s=(result[0].money_sum[no2]-result[0].money_sum[no1]);
            Time ar=x+(result[0].time_sum[no2]-result[0].time_sum[no1]-result[0].stopoverTimes[no2-1]);
            order_base.insert(username(u),order(times,day_,success,i,f,no1,t,no2,
                    money_s,n,x,ar));
            long long ansss=money_s*n;
            return ansss;
        }
        else{
            //记入用户的订单信息
            if(q==false){
                // std::cout<<333<<std::endl;
                return -1;
            }
            int money_s=(result[0].money_sum[no2]-result[0].money_sum[no1]);
            Time ar=x+(result[0].time_sum[no2]-result[0].time_sum[no1]-result[0].stopoverTimes[no2-1]);
            order_base.insert(username(u),order(times,day_,pending,i,f,no1,t,no2,
                    money_s,n,x,ar));
            //加入候补队列
            candidate_base.insert(train_f(i,day_),candidate(times,u,no1,no2,n));
            return -2;
        }
    }
    int query_order(char* u,train &trains,user &users){
        if(users.login_set.find(username(u))==users.login_set.end())return -1;
        sjtu::vector<order> result=order_base.find(username(u));
        std::cout<<result.size()<<'\n';
        for(int i=0;i<result.size();i++){
            if(result[i].status_==success)std::cout<<"[success] ";
            else if(result[i].status_==pending)std::cout<<"[pending] ";
            else std::cout<<"[refunded] ";
            std::cout<<result[i].trainID<<' '<<result[i].from<<' '<<result[i].leaving.toString()<<" -> "
                     <<result[i].to<<' '<<result[i].arriving.toString()<<' '<<result[i].price<<' '<<result[i].num<<'\n'; 
        }
        return 1;
    }
    int refund_ticket(char* u,int n,train &trains,user &users,ticket_base &ticket_base_){
        if(users.login_set.find(username(u))==users.login_set.end())return -1;
        sjtu::vector<order> result=order_base.find(username(u));
        if(result.size()<n || result[n-1].status_==refunded)return -1;
        if(result[n-1].status_==pending){
            order_base.erase(username(u),result[n-1]);
            result[n-1].status_=refunded;
            order_base.insert(username(u),result[n-1]);
            //移出候补队列
            candidate_base.erase(train_f(result[n-1].trainID,result[n-1].date),
                candidate(result[n-1].time,u,result[n-1].no1,result[n-1].no2,result[n-1].num));//从候补列表删除
        }
        else{
            //订单状态更新
            order_base.erase(username(u),result[n-1]);
            result[n-1].status_=refunded;
            order_base.insert(username(u),result[n-1]);
            //火车余票更新
            // sjtu::vector<train_inf> results=trains.train_base.find(result[n-1].trainID);
            // trains.train_base.erase(result[n-1].trainID,results[0]);
            ticket_left left_temp(ticket_base_.query(train_f(result[n-1].trainID,result[n-1].date)));
            for(int i=result[n-1].no1;i<result[n-1].no2;i++)
                left_temp.ticket[i]+=result[n-1].num;
            //候补尝试
            sjtu::vector<candidate> waiting=
                candidate_base.find(train_f(result[n-1].trainID,result[n-1].date));
            for(int i=0;i<waiting.size();i++){
                int seats_left=200000;
                for(int j=waiting[i].no1;j<waiting[i].no2;j++)
                    seats_left=std::min(seats_left,left_temp.ticket[j]);
                if(seats_left>=waiting[i].n){
                    for(int j=waiting[i].no1;j<waiting[i].no2;j++)
                        left_temp.ticket[j]-=waiting[i].n;
                    sjtu::vector<order> orders=order_base.find(waiting[i].u);
                    int l=0,r=orders.size()-1,mid;//找到时间戳为waiting[i].time,在第mid位
                    while(l<=r){
                        mid=(l+r)>>1;
                        if(waiting[i].time==orders[mid].time)break;
                        if(waiting[i].time<orders[mid].time)l=mid+1;
                        else r=mid-1;
                    }
                    order_base.erase(waiting[i].u,orders[mid]);
                    orders[mid].status_=success;
                    order_base.insert(waiting[i].u,orders[mid]);
                    candidate_base.erase(train_f(result[n-1].trainID,result[n-1].date),waiting[i]);
                }
            }
            //最后记得把results[0]写回去
            // trains.train_base.insert(result[n-1].trainID,results[0]);
            ticket_base_.modify(train_f(result[n-1].trainID,result[n-1].date),left_temp);
        }
        return 0;
    }
};


#endif