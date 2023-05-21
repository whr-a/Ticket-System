#ifndef TRAINS
#define TRAINS
#include <string>
#include <cstring>
#include <iostream>
#include "map.hpp"
#include "database_copy.hpp"
#include "utility.hpp"
#include "ticket_left.hpp"
#include "train_database.hpp"
class trainID
{
public:
    char ID[21];
    trainID(){ID[0]=0;}
    trainID(const trainID &obj){
        strcpy(ID,obj.ID);
    }
    trainID(char *obj){strcpy(ID,obj);}
    friend bool operator < (const trainID &obj1,const trainID &obj2) {
        return strcmp(obj1.ID,obj2.ID)<0;
    }
    friend bool operator <= (const trainID &obj1,const trainID &obj2) {
        return strcmp(obj1.ID,obj2.ID)<=0;
    }
    friend bool operator > (const trainID &obj1,const trainID &obj2) {
        return strcmp(obj1.ID,obj2.ID)>0;
    }
    friend bool operator == (const trainID &obj1,const trainID &obj2) {
        return strcmp(obj1.ID,obj2.ID)==0;
    }
    friend bool operator != (const trainID &obj1,const trainID &obj2) {
        return strcmp(obj1.ID,obj2.ID)!=0;
    }
    friend std::ostream & operator<<(std::ostream &os,const trainID &obj){
        os<<obj.ID;
        return os;
    }
    trainID operator = (const trainID &obj){
        strcpy(ID,obj.ID);
        return *this;
    }
};


class train_inf
{
public:
    char trainID[21];
    int stationNum;
    int seatNum;
    char stations[101][33];
    int prices[101];
    sjtu::daytime startTime;
    int travelTimes[101];
    int stopoverTimes[101];
    int time_sum[101];
    int money_sum[101];
    sjtu::monthtime saleDate[2];
    char type;
    bool release;
    train_inf(){trainID[0]=0;release=0;}
    train_inf(const train_inf &other){
        strcpy(trainID,other.trainID);
        stationNum=other.stationNum;
        seatNum=other.seatNum;
        for(int i=0;i<101;i++){
            strcpy(stations[i],other.stations[i]);
            prices[i]=other.prices[i];
            travelTimes[i]=other.travelTimes[i];
            stopoverTimes[i]=other.stopoverTimes[i];
            time_sum[i]=other.time_sum[i];
            money_sum[i]=other.money_sum[i];
            type=other.type;
            release=other.release;
        }
        startTime=other.startTime;
        saleDate[0]=other.saleDate[0];
        saleDate[1]=other.saleDate[1];
    }
    train_inf operator= (const train_inf &other){
        strcpy(trainID,other.trainID);
        stationNum=other.stationNum;
        seatNum=other.seatNum;
        for(int i=0;i<101;i++){
            strcpy(stations[i],other.stations[i]);
            prices[i]=other.prices[i];
            travelTimes[i]=other.travelTimes[i];
            stopoverTimes[i]=other.stopoverTimes[i];
            time_sum[i]=other.time_sum[i];
            money_sum[i]=other.money_sum[i];
            type=other.type;
            release=other.release;
        }
        startTime=other.startTime;
        saleDate[0]=other.saleDate[0];
        saleDate[1]=other.saleDate[1];
        return *this;
    }
    train_inf(char* trainID_,int stn,int sen,std::string* stations_,int* price,
        sjtu::daytime startTime_,int* traveltimes,int* stopoverTimes_,sjtu::monthtime* saleDate_,char type_,int *time_s,int *money_s){
        strcpy(trainID,trainID_);
        stationNum=stn;
        seatNum=sen;
        for(int i=0;i<stationNum;i++){
            strcpy(stations[i],stations_[i].c_str());
            time_sum[i]=time_s[i];
            money_sum[i]=money_s[i];
            prices[i]=price[i];
            travelTimes[i]=traveltimes[i];
            stopoverTimes[i]=stopoverTimes_[i];;
        }
        startTime=startTime_;
        saleDate[0]=saleDate_[0];
        saleDate[1]=saleDate_[1];
        type=type_;
        release=0;
    }
    friend bool operator< (const train_inf &obj1,const train_inf &obj2){
        return strcmp(obj1.trainID,obj2.trainID)<0;
    }
};
class station
{
public:
    char name[33];
    station(){name[0]=0;}
    station(char *obj){
        strcpy(name,obj);
    }
    station(const std::string &s){
        strcpy(name,s.c_str());
    }
    friend bool operator<(const station &obj1,const station &obj2){
        return strcmp(obj1.name,obj2.name)<0;
    }
    friend bool operator<=(const station &obj1,const station &obj2){
        return strcmp(obj1.name,obj2.name)<=0;
    }
    friend bool operator == (const station &obj1,const station &obj2) {
        return strcmp(obj1.name,obj2.name)==0;
    }
    friend bool operator != (const station &obj1,const station &obj2) {
        return strcmp(obj1.name,obj2.name)!=0;
    }
}; 
class bundle
{
public:
    trainID ID;
    int pos;
    bundle(){ID.ID[0]=0;}
    bundle(trainID ID_,int pos_):ID(ID_),pos(pos_){}
    friend bool operator< (const bundle &obj1,const bundle &obj2){
        return strcmp(obj1.ID.ID,obj2.ID.ID)<0;
    }
};
class sorter
{
public:
    int standard;
    char id[21];
    std::string s;
    sorter(){id[0]=0;}
    sorter(int standard_,char* id_,std::string s_):standard(standard_){
        strcpy(id,id_);
        s=s_;
    }
    friend bool operator<(const sorter &obj1,const sorter &obj2){
        if(obj1.standard<obj2.standard)return true;
        else if(obj1.standard==obj2.standard && strcmp(obj1.id,obj2.id)<0)return true;
        else return false;
    }
};
class train
{
public:

    database<trainID,int> train_base;
    database<station,bundle> station_base;
    train_database<train_inf> train_inf_base;
    train(){
        train_base.setfile("train.db");
        station_base.setfile("station.db");
    }
    void clear(){
        train_base.clear("train.db");
        station_base.clear("train.db");
        train_inf_base.clear();
    }
    int addtrain(char* trainid,int stationnum,int seatnum,std::string* stations_,int* prices_,
        sjtu::daytime starttime,int* traveltimes,int* stopovertimes,sjtu::monthtime* saledate,char type_,ticket_base &ticket_base_){
            //std::cout<<"******"<<std::endl<<starttime.hh<<' '<<starttime.mm<<std::endl;
            sjtu::vector<int> ansss=train_base.find(trainID(trainid));
            if(!ansss.empty())return -1;
            
            int sum1[101],sum2[101],left_temp[101];
            sum1[0]=0;sum2[0]=0;
            //station_base.insert(station(stations_[0]),bundle(trainid,0));
            left_temp[0]=seatnum;
            // std::cout<<"**************\n";
            for(int i=1;i<stationnum;i++){
                left_temp[i]=seatnum;
                sum1[i]=sum1[i-1]+traveltimes[i-1]+stopovertimes[i-1];
                // std::cout<<sum1[i]<<'\n';
                sum2[i]=sum2[i-1]+prices_[i-1];
                //station_base.insert(station(stations_[i]),bundle(trainid,i));
            }
            // std::cout<<"**************"<<std::endl;
            int xx=train_inf_base.insert(train_inf(trainid,stationnum,seatnum,stations_,
            prices_,starttime,traveltimes,stopovertimes,saledate,type_,sum1,sum2));
            train_base.insert(trainID(trainid),xx);
            // Time obj1(saledate[0]),obj2(saledate[1]);
            // int xx=obj2.days_to(obj1);
            // for(int i=0;i<=xx;i++){
            //     ticket_base_.add(train_f(trainid,i),ticket_left(left_temp,stationnum));
            // }
            return 0;
    }
    int delete_train(char* trainid,ticket_base &ticket_base_){
        sjtu::vector<int> ansss=train_base.find(trainID(trainid));
        if(ansss.empty())return -1;
        train_inf ans=train_inf_base.find(ansss[0]);

        if(ans.release)return -1;
        // int left_temp[101];//剩余票数
        // for(int i=0;i<ans[0].stationNum;i++)left_temp[i]=ans[0].seatNum;//剩余票数都为座位数
        // Time obj1(ans[0].saleDate[0]),obj2(ans[0].saleDate[1]);
        // int xx=obj2.days_to(obj1);//两个日期之差
        // for(int i=0;i<=xx;i++){
        //     ticket_base_.erase(train_f(trainid,i),ticket_left(left_temp,ans[0].stationNum));
        // }
        train_base.erase(trainid,ansss[0]);
        train_inf_base.erase(ansss[0]);
        return 0;
    }
    int release_train(char* trainid,ticket_base &ticket_base_){
        // train_base.prints();
        sjtu::vector<int> ansss=train_base.find(trainID(trainid));
        if(ansss.empty()){
            //std::cout<<111<<std::endl;
            return -1;
        }
        train_inf ans=train_inf_base.find(ansss[0]);
        if(ans.release){
            //std::cout<<222<<std::endl;
            return -1;
        }

        train_base.erase(trainid,ansss[0]);
        train_inf_base.erase(ansss[0]);
        ans.release=1;
        int x_temp=train_inf_base.insert(ans);
        train_base.insert(trainid,x_temp);//可优化
        //把站->火车处理
        for(int i=0;i<ans.stationNum;i++){
            station_base.insert(station(ans.stations[i]),bundle(trainid,i));
        }
        //发票
        Time obj1(ans.saleDate[0]),obj2(ans.saleDate[1]);
        int xx=obj2.days_to(obj1);
        int left_temp[101];
        for(int i=0;i<ans.stationNum;i++){
            left_temp[i]=ans.seatNum;
        }
        for(int i=0;i<=xx;i++){
            ticket_base_.add(train_f(trainid,i),ticket_left(left_temp,ans.stationNum));
        }
        return 0;
    }
    int query_train(char* trainid,int month,int day,ticket_base &ticket_base_){
        sjtu::vector<int> ansss=train_base.find(trainID(trainid));
        if(ansss.empty())return -1;
        train_inf ans=train_inf_base.find(ansss[0]);

        
        if(ans.saleDate[0]>sjtu::monthtime(month,day)
            || ans.saleDate[1]<sjtu::monthtime(month,day)){
                // std::cout<<222<<std::endl;
                return -1;
            }
        int xx=Time(month,day,0,0).days_to(Time(ans.saleDate[0]));
        
        ticket_left left_temp;
        if(!ans.release){
            for(int i=0;i<ans.stationNum;i++)left_temp.ticket[i]=ans.seatNum;
        }
        else{
            left_temp=ticket_base_.query(train_f(trainid,xx));
        }
        std::cout<<trainid<<' '<<ans.type<<'\n';
        std::cout<<ans.stations[0]<<" xx-xx xx:xx -> "
                 <<Time(month,day,ans.startTime.hh,ans.startTime.mm).toString()<<" 0 "
                 <<left_temp.ticket[0]<<'\n';
        Time temp(month,day,ans.startTime.hh,ans.startTime.mm);
        int price=0;
        for(int i=1;i<ans.stationNum-1;i++){
            price+=ans.prices[i-1];
            temp=temp+ans.travelTimes[i-1];
            std::cout<<ans.stations[i]<<' '
                     <<temp.toString()<<" -> ";
            temp=temp+ans.stopoverTimes[i-1];
            std::cout<<temp.toString()<<' '
                     <<price<<' '
                     <<left_temp.ticket[i]<<'\n';
        }
        price+=ans.prices[ans.stationNum-2];
        temp=temp+ans.travelTimes[ans.stationNum-2];
        std::cout<<ans.stations[ans.stationNum-1]<<' '<<temp.toString()
                 <<" -> xx-xx xx:xx "<<price<<" x\n";
        return 1;
    }
    void query_ticket(char* s,char* t,int month,int day,bool p,ticket_base &ticket_base_){//p==0 为time，p==1为cost
        sjtu::vector<bundle> ans1=station_base.find(station(s));//找到经过出发站的所有车
        sjtu::vector<bundle> ans2=station_base.find(station(t));//找到经过到达站的所有车
        // std::cout<<ans1.size()<<' '<<ans2.size()<<std::endl;
        int pointer1=0,pointer2=0;//双指针
        sjtu::vector<sorter> anss;//答案存在里面
        while(pointer1<ans1.size() && pointer2<ans2.size()){//搜索
            //if(ans1.size()==1 && ans2.size()==5)std::cout<<pointer1<<' '<<pointer2<<std::endl;
            if(ans1[pointer1].ID<ans2[pointer2].ID){
                ++pointer1;
                continue;
            }
            else if(ans1[pointer1].ID>ans2[pointer2].ID){
                ++pointer2;
                continue;
            }
            else{//找到了同时经过这两站的车，id为ans1[pointer1].ID

                if(ans1[pointer1].pos>=ans2[pointer2].pos){//顺序反了
                    ++pointer1;
                    ++pointer2;
                    continue;
                }
                sjtu::vector<int> pos=train_base.find(ans1[pointer1].ID);
                // if(pos.empty()){
                //     ++pointer1;
                //     ++pointer2;
                //     continue;
                // }//好像不需要？

                train_inf result=train_inf_base.find(pos[0]);//把这辆车调出来，存在result里
                
                if(!result.release){
                    ++pointer1;
                    ++pointer2;
                    continue;
                }
                Time x(7,15,result.startTime.hh,result.startTime.mm);//始发站时间
                int time1=result.time_sum[ans1[pointer1].pos];//到出发站的时间
                x=x+time1;//x为到出发站的时间
                x.setMonth(month);x.setDay(day);//设置月份和日份
                x=x-time1;//始发站时间
                Time st(result.saleDate[0],result.startTime),
                        ed(result.saleDate[1],result.startTime);
                if(x<st || x>ed){
                    ++pointer1;
                    ++pointer2;
                    continue;//判断是否合法
                }
                //std::cout<<"******"<<std::endl<<result[0].startTime.hh<<' '<<result[0].startTime.mm<<std::endl;
                int time_sum=result.time_sum[ans2[pointer2].pos]-result.time_sum[ans1[pointer1].pos]
                    -result.stopoverTimes[ans2[pointer2].pos-1];//两站之间的时间
                int price_sum=result.money_sum[ans2[pointer2].pos]-result.money_sum[ans1[pointer1].pos];
                int no=x.days_to(Time(result.saleDate[0]));//第几趟车
                ticket_left left_temp(ticket_base_.query(train_f(ans1[pointer1].ID.ID,no)));
                x=x+time1;//出发站时间
                Time x2=x+time_sum;//到达站时间
                int seats=200000;
                for(int i=ans1[pointer1].pos;i<ans2[pointer2].pos;i++){
                    seats=std::min(seats,left_temp.ticket[i]);//找座位最小值
                }
                std::string ans=std::string(ans1[pointer1].ID.ID)+" "+std::string(s)+" "+x.toString()+" -> "+
                    std::string(t)+" "+x2.toString()+" "+std::to_string(price_sum)+" "+std::to_string(seats);
                if(p){
                    anss.push_back(sorter(price_sum,ans1[pointer1].ID.ID,ans));
                }
                else{
                    anss.push_back(sorter(time_sum,ans1[pointer1].ID.ID,ans));
                }
                ++pointer1;
                ++pointer2;
            }
        }
        //std::cout<<114514<<std::endl;
        anss.sort();
        std::cout<<anss.size()<<'\n';
        for(int i=0;i<anss.size();i++)std::cout<<anss[i].s<<'\n';
    }
    int query_transfer(char* s,char* t,int month,int day,bool p,ticket_base &ticket_base_){//p==0 为time，p==1为cost
        sjtu::vector<bundle> ans1=station_base.find(station(s));//经过首站的车
        sjtu::vector<bundle> ans3=station_base.find(station(t));//经过尾站的车
        int time_summm;//时间总和
        int cost_summm;//花费总和
        std::string t1_,t2_;//存答案
        bool flag3=0;
        for(int i=0;i<ans1.size();i++){//遍历所有经过出发站的车
            sjtu::vector<int> pos=train_base.find(ans1[i].ID);
            train_inf result=train_inf_base.find(pos[0]);//第一辆车的信息
            if(!result.release)continue;//没发布则不行
            Time xxx(7,15,result.startTime.hh,result.startTime.mm);//发车时间
            int time11=result.time_sum[ans1[i].pos];//第一辆车到出发站的时间
            xxx=xxx+time11;//从出发站发车时间
            xxx.setMonth(month);
            xxx.setDay(day);
            xxx=xxx-time11;//得到第一辆车从始发站的发车时间
            Time st(result.saleDate[0],result.startTime),
                    ed(result.saleDate[1],result.startTime);
            if(xxx<st || xxx>ed)continue;//时间不合法
            int no_=xxx.days_to(Time(result.saleDate[0]));//第一辆车是第几班
            xxx=xxx+time11;//这是从s站发车的时间
            int time_summ;
            int cost_summ;
            std::string t1,t2;//存答案
            bool flag2=0;
            for(int j=ans1[i].pos+1;j<result.stationNum;j++){//遍历这辆车经过的所有站，作为中转站 ans1[i].pos为s的位置
                int time_first=result.time_sum[j]-result.time_sum[ans1[i].pos]-result.stopoverTimes[j-1];//第一辆车到中转站的时间
                int cost_first=result.money_sum[j]-result.money_sum[ans1[i].pos];//第一辆车到中转站的花费
                sjtu::vector<bundle> ans2=station_base.find(station(result.stations[j]));//经过中转站的所有车
                int pointer1=0,pointer2=0;
                int time_s,cost_s;
                trainID id_;
                bool flag=0;
                std::string ans;
                Time mid_time=xxx+time_first;//这是到达中转站的时间
                while(pointer1<ans2.size() && pointer2<ans3.size()){//遍历从中转站到终点站的所有车
                    if(ans2[pointer1].ID<ans3[pointer2].ID){
                        pointer1++;
                        continue;
                    }
                    else if(ans2[pointer1].ID>ans3[pointer2].ID){
                        pointer2++;
                        continue;
                    }
                    else{//找到了第二辆车，为ans2[pointer1].ID
                        if(ans2[pointer1].pos>=ans3[pointer2].pos){
                            ++pointer1;
                            ++pointer2;
                            continue;
                        }
                        if(strcmp(ans1[i].ID.ID,ans2[pointer1].ID.ID)==0){//转的车和前面的车是同一辆车
                            ++pointer1;
                            ++pointer2;
                            continue;
                        }
                        sjtu::vector<int> pos_2=train_base.find(ans2[pointer1].ID);
                        train_inf results=train_inf_base.find(pos_2[0]);//经过中转站的这辆车已找到
                        if(!results.release){//这辆车没放出来
                            ++pointer1;
                            ++pointer2;
                            continue;
                        }
                        Time x(7,15,results.startTime.hh,results.startTime.mm);//第二辆车的始发时间
                        int time1=results.time_sum[ans2[pointer1].pos];//第二辆车到中转站的时间
                        x=x+time1;//第二辆车从中转站出发的时间
                        x.setMonth(mid_time.getMonth());x.setDay(mid_time.getDay());//这里出问题了
                        if(x<mid_time)x=x+24*60;
                        Time st(results.saleDate[0],results.startTime),
                                ed(results.saleDate[1],results.startTime);
                        x=x-time1;
                        if(x>ed){
                            ++pointer1;
                            ++pointer2;
                            continue;
                        }
                        if(x<st)x=st;
                        int no=x.days_to(Time(results.saleDate[0]));
                        x=x+time1;//从中转站发车时间
                        int time_sum=results.time_sum[ans3[pointer2].pos]-results.time_sum[ans2[pointer1].pos]
                            -results.stopoverTimes[ans3[pointer2].pos-1]+(x-mid_time);//中转站到t之间的时间
                        int time_sum_=results.time_sum[ans3[pointer2].pos]-results.time_sum[ans2[pointer1].pos]
                            -results.stopoverTimes[ans3[pointer2].pos-1];
                        int price_sum=results.money_sum[ans3[pointer2].pos]-results.money_sum[ans2[pointer1].pos];
                        if(flag){
                            if(p){
                                if(!(price_sum<cost_s || (price_sum==cost_s && time_sum<time_s))){
                                    ++pointer1;
                                    ++pointer2;
                                    continue;
                                }
                            }else{
                                if(!(time_sum<time_s || (time_sum==time_s && price_sum<cost_s))){
                                    ++pointer1;
                                    ++pointer2;
                                    continue;
                                }
                            }
                        }
                        else flag=1;//找到了满足条件的第二辆车
                        Time x2=x+time_sum_;//到达终点站的时间
                        int seats=200000;
                        ticket_left left_temp(ticket_base_.query(train_f(ans2[pointer1].ID.ID,no)));//no为第二辆车是第几班
                        for(int k=ans2[pointer1].pos;k<ans3[pointer2].pos;k++){
                            seats=std::min(seats,left_temp.ticket[k]);
                        }
                        ans=std::string(ans2[pointer1].ID.ID)+" "+std::string(result.stations[j])+" "+x.toString()+" -> "+
                            std::string(t)+" "+x2.toString()+" "+std::to_string(price_sum)+" "+std::to_string(seats);
                        cost_s=price_sum;
                        time_s=time_sum;
                        //id_=ans2[pointer1].ID;//好像不用存
                        ++pointer1;
                        ++pointer2;
                    }
                }
                if(!flag)continue;//以j为中转站不可行
                int time_sums=time_first+time_s;
                int cost_sums=cost_first+cost_s;

                if(flag2){
                    if(p){
                        if(!(cost_sums<cost_summ || (cost_sums==cost_summ && time_sums<time_summ)))continue;
                    }else{
                        if(!(time_sums<time_summ || (time_sums==time_summ && cost_sums<cost_summ)))continue;
                    }
                }
                else flag2=1;
                int seatsss=200000;
                ticket_left left_temp_(ticket_base_.query(train_f(ans1[i].ID.ID,no_)));
                for(int k=ans1[i].pos;k<j;k++){
                    seatsss=std::min(seatsss,left_temp_.ticket[k]);
                }
                t1=std::string(ans1[i].ID.ID)+" "+std::string(s)+" "+xxx.toString()+" -> "+
                    std::string(result.stations[j])+" "+mid_time.toString()+" "+std::to_string(cost_first)+" "+std::to_string(seatsss);
                t2=ans;
                time_summ=time_sums;
                cost_summ=cost_sums;
            }
            if(!flag2)continue;//以i为第一辆车不可行
            if(flag3){
                if(p){
                    if(!(cost_summ<cost_summm || (cost_summ==cost_summm && time_summ<time_summm)))continue;
                }else{
                    if(!(time_summ<time_summm || (time_summ==time_summm && cost_summ<cost_summm)))continue;
                }
            }
            else flag3=1;
            t1_=t1;
            t2_=t2;
            time_summm=time_summ;
            cost_summm=cost_summ;
        }
        if(!flag3)return 0;
        std::cout<<t1_<<'\n'<<t2_<<'\n';
        return 1;
    }
};
// trainID：车次的唯一标识符，由字母开头，字母、数字和下划线组成的字符串，长度不超过 20。
// stationNum：车次经过的车站数量，一个不低于 2 且不超过 100 的整数。
// stations：车次经过的所有车站名，共 stationNum 项，每个站名由汉字组成，不超过 10 个汉字。
// seatNum：该车次的座位数，一个不超过 
//  的整数。
// prices：每两站之间的票价，共 (stationNum - 1) 项，第 i 项表示第 i 站到第 (i+1) 站的票价，为一个不超过 
//  的整数。
// startTime：列车每日的发车时间。
// 时间格式为 hh:mm ，例如 23:51
// travelTimes：每两站之间行车所用的时间，共 (stationNum - 1) 项。
// 单位：分钟
// 每一项是一个不超过 
//  的整数
// stopoverTimes：除始发站和终点站之外，列车在每一站停留的时间，共 (stationNum - 2) 项。
// 单位：分钟
// 每一项为一个不超过 
//  的整数
// saleDate：车次的售卖时间区间。
// 由 2 项时间表示，每一项均为 2023 年 6 月至 8 月的某一日期
// 日期格式为 mm-dd，例如：06-01、08-31
// type：列车类型，一个大写字母。
#endif