#ifndef TRAINS
#define TRAINS
#include <string>
#include <cstring>
#include "map.hpp"
#include "database.hpp"
#include "utility.hpp"

class trainID
{
public:
    char ID[21];
    trainID(){}
    trainID(char *obj){memcpy(ID,obj,21);}
    friend bool operator < (const trainID &obj1,const trainID &obj2) {
        return strcmp(obj1.ID,obj2.ID)<0;
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
    // trainID operator = (const trainID &obj){
    //     memcpy(ID,obj.ID,21);
    //     return *this;
    // }
};


class train_inf
{
public:
    char trainID[21];
    int stationNum;
    int seatNum;
    int ticket_left[100][101];
    char stations[101][33];
    int prices[100];
    sjtu::daytime startTime;
    int travelTimes[101];
    int stopoverTimes[101];
    int time_sum[101];
    int money_sum[101];
    sjtu::monthtime saleDate[2];
    char type;
    bool release;
    train_inf(){}
    train_inf(char* trainID_,int stn,int sen,std::string* stations_,int* price,
        sjtu::daytime startTime_,int* traveltimes,int* stopoverTimes_,sjtu::monthtime* saleDate_,char type_,int *time_s,int *money_s){
        memcpy(trainID,trainID_,21);
        stationNum=stn;
        seatNum=sen;
        memcpy(time_sum,time_s,stationNum);
        memcpy(money_sum,money_s,stationNum);
        for(int i=0;i<stationNum;i++){
            strcpy(stations[i],stations_[i].c_str());
            for(int j=0;j<100;j++){
                ticket_left[j][i]=seatNum;
            }
        }
        memcpy(prices,price,100);
        startTime=startTime_;
        memcpy(travelTimes,traveltimes,101);
        memcpy(stopoverTimes,stopoverTimes_,101);
        saleDate[0]=saleDate_[0];
        saleDate[1]=saleDate_[1];
        type=type_;
        release=0;
    }
};
class station
{
public:
    char name[33];
    station(){}
    station(char *obj){
        memcpy(name,obj,33);
    }
    station(const std::string &s){
        strcpy(name,s.c_str());
    }
    friend bool operator<(const station &obj1,const station &obj2){
        return strcmp(obj1.name,obj2.name)<0;
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
    bundle(){}
    bundle(trainID ID_,int pos_):ID(ID_),pos(pos_){}
};
class sorter
{
public:
    int standard;
    char id[21];
    std::string s;
    sorter(){}
    sorter(int standard_,char* id_,std::string s_):standard(standard_){
        memcpy(id,id_,21);
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
    database<trainID,train_inf> train_base;
    database<station,bundle> station_base;
    train(){
        train_base.setfile("train.db");
    }
    void clear(){
        train_base.clear("train.db");
    }
    int addtrain(char* trainid,int stationnum,int seatnum,std::string* stations_,int* prices_,
        sjtu::daytime starttime,int* traveltimes,int* stopovertimes,sjtu::monthtime* saledate,char type_){
            sjtu::vector<train_inf> ans=train_base.find(trainid);
            if(!ans.empty())return -1;
            int sum1[101],sum2[101];
            sum1[0]=0;sum2[0]=0;
            // char stations_[101][33];
            // for(int i=0;i<stationnum;i++)strcpy(stations_[i],stations[i].c_str());
            station_base.insert(station(stations_[0]),bundle(trainid,0));
            for(int i=1;i<stationnum;i++){
                sum1[i]=sum1[i-1]+traveltimes[i-1]+stopovertimes[i-1];
                sum2[i]=sum2[i-1]+prices_[i-1];
                station_base.insert(station(stations_[i]),bundle(trainid,i));
            }
            train_base.insert(trainid,train_inf(trainid,stationnum,seatnum,stations_,
            prices_,starttime,traveltimes,stopovertimes,saledate,type_,sum1,sum2));
            return 0;
    }
    int delete_train(char* trainid){
        sjtu::vector<train_inf> ans=train_base.find(trainid);
        if(ans.empty())return -1;
        if(ans[0].release)return -1;
        train_base.erase(trainid,ans[0]);
        return 0;
    }
    int release_train(char* trainid){
        sjtu::vector<train_inf> ans=train_base.find(trainid);
        if(ans.empty())return -1;
        train_base.erase(trainid,ans[0]);
        ans[0].release=1;
        train_base.insert(trainid,ans[0]);//可优化
        return 0;
    }
    int query_train(char* trainid,int month,int day){
        sjtu::vector<train_inf> ans=train_base.find(trainid);
        if(ans.empty())return -1;
        if(Time(ans[0].saleDate[0])>Time(month,day,0,0)
            || Time(ans[0].saleDate[1])<Time(month,day,0,0))return -1;
        std::cout<<trainid<<' '<<ans[0].type<<'\n';
        std::cout<<ans[0].stations[0]<<" xx-xx xx:xx -> "
                 <<Time(month,day,ans[0].startTime.hh,ans[0].startTime.mm).toString()<<" 0 "
                 <<ans[0].ticket_left[0]<<'\n';
        Time temp(month,day,ans[0].startTime.hh,ans[0].startTime.mm);
        int price=0;
        for(int i=1;i<ans[0].stationNum-1;i++){
            price+=ans[0].prices[i-1];
            temp=temp+ans[0].travelTimes[i-1];
            std::cout<<ans[0].stations[i]<<' '
                     <<temp.toString()<<" -> ";
            temp=temp+ans[0].stopoverTimes[i-1];
            std::cout<<temp.toString()<<' '
                     <<price<<' '
                     <<ans[0].ticket_left[i]<<'\n';
        }
        price+=ans[0].prices[ans[0].stationNum-2];
        temp=temp+ans[0].travelTimes[ans[0].stationNum-2];
        std::cout<<ans[0].stations[ans[0].stationNum-1]<<' '<<temp.toString()
                 <<" -> xx-xx xx:xx "<<price<<" x\n";
        return 1;
    }
    void query_ticket(char* s,char* t,int month,int day,bool p){//p==0 为time，p==1为cost
        sjtu::vector<bundle> ans1=station_base.find(station(s));
        sjtu::vector<bundle> ans2=station_base.find(station(t));
        int pointer1=0,pointer2=0;
        sjtu::vector<sorter> anss;
        while(pointer1<ans1.size() && pointer2<ans2.size()){
            if(ans1[pointer1].ID<ans2[pointer2].ID){
                pointer1++;
                continue;
            }
            else if(ans1[pointer1].ID>ans2[pointer2].ID){
                pointer2++;
                continue;
            }
            else{
                if(ans1[pointer1].pos>=ans2[pointer2].pos)continue;
                sjtu::vector<train_inf> result=train_base.find(ans1[pointer1].ID);
                Time x(7,15,result[0].startTime.hh,result[0].startTime.mm);
                int time1=result[0].time_sum[ans1[pointer1].pos];
                x=x+time1;
                x.setMonth(month);x.setDay(day);
                x=x-time1;
                Time st(result[0].saleDate[0],result[0].startTime),
                        ed(result[0].saleDate[1],result[0].startTime);
                if(x<st || x>ed)continue;
                int time_sum=result[0].time_sum[ans2[pointer2].pos]-result[0].time_sum[ans1[pointer1].pos]
                    -result[0].stopoverTimes[ans2[pointer2].pos-1];//两站之间的时间
                int price_sum=result[0].money_sum[ans2[pointer2].pos]-result[0].money_sum[ans1[pointer1].pos];
                int no=x.days_to(Time(result[0].saleDate[0]));
                x=x+time1;
                Time x2=x+time_sum;
                int seats=200000;
                for(int i=ans1[pointer1].pos;i<ans2[pointer2].pos;i++){
                    seats=std::min(seats,result[0].ticket_left[no][i]);
                }
                std::string ans=std::string(ans1[pointer1].ID.ID)+" "+std::string(s)+" "+x.toString()+" -> "+
                    std::string(t)+" "+x2.toString()+" "+std::to_string(price_sum)+" "+std::to_string(seats);
                if(p){
                    anss.push_back(sorter(price_sum,ans1[pointer1].ID.ID,ans));
                }
                else{
                    anss.push_back(sorter(time_sum,ans1[pointer1].ID.ID,ans));
                }
            }
        }
        anss.sort();
        std::cout<<anss.size()<<'\n';
        for(int i=0;i<anss.size();i++)std::cout<<anss[i].s<<'\n';
    }
    int query_transfer(char* s,char* t,int month,int day,bool p){//p==0 为time，p==1为cost
        sjtu::vector<bundle> ans1=station_base.find(station(s));
        sjtu::vector<bundle> ans3=station_base.find(station(t));
        int time_summm;
        int cost_summm;
        std::string t1_,t2_;
        bool flag3=0;
        for(int i=0;i<ans1.size();i++){//遍历所有经过出发站的车
            sjtu::vector<train_inf> result=train_base.find(ans1[i].ID);//第一辆车的信息
            Time xxx(7,15,result[0].startTime.hh,result[0].startTime.mm);
            int time11=result[0].time_sum[ans1[i].pos];
            xxx=xxx+time11;
            xxx.setMonth(month);
            xxx.setDay(day);
            xxx=xxx-time11;
            Time st(result[0].saleDate[0],result[0].startTime),
                    ed(result[0].saleDate[1],result[0].startTime);
            if(xxx<st || xxx>ed)continue;
            int no_=xxx.days_to(Time(result[0].saleDate[0]));
            xxx=xxx+time11;//这是从s站发车的时间
            int time_summ;
            int cost_summ;
            std::string t1,t2;
            bool flag2=0;
            for(int j=ans1[i].pos+1;j<result[0].stationNum;j++){//遍历这辆车经过的所有站，作为中转站
                int time_first=result[0].time_sum[j]-result[0].time_sum[ans1[i].pos]-result[0].stopoverTimes[j-1];
                int cost_first=result[0].money_sum[j]-result[0].money_sum[ans1[i].pos];
                sjtu::vector<bundle> ans2=station_base.find(station(result[0].stations[j]));//经过中转站的所有车
                int pointer1=0,pointer2=0;
                int time_s,cost_s;
                trainID id_;
                bool flag=0;
                std::string ans;
                Time mid_time=xxx+time_first;
                while(pointer1<ans2.size() && pointer2<ans3.size()){//遍历从中转站到终点站的所有车
                    if(ans2[pointer1].ID<ans3[pointer2].ID){
                        pointer1++;
                        continue;
                    }
                    else if(ans2[pointer1].ID>ans3[pointer2].ID){
                        pointer2++;
                        continue;
                    }
                    else{
                        if(ans2[pointer1].pos>=ans3[pointer2].pos)continue;
                        if(strcmp(ans1[i].ID.ID,ans2[pointer1].ID.ID)==0)continue;
                        sjtu::vector<train_inf> results=train_base.find(ans2[pointer1].ID);//经过中转站的这辆车已找到
                        Time x(7,15,results[0].startTime.hh,results[0].startTime.mm);
                        int time1=results[0].time_sum[ans2[pointer1].pos];
                        x=x+time1;
                        x.setMonth(mid_time.getMonth());x.setDay(mid_time.getDay());
                        if(x<=mid_time)x=x+24*60;
                        x=x-time1;
                        Time st(results[0].saleDate[0],results[0].startTime),
                                ed(results[0].saleDate[1],results[0].startTime);
                        if(x<st || x>ed)continue;
                        int no=x.days_to(Time(results[0].saleDate[0]));
                        x=x+time1;//从中转站发车时间
                        int time_sum=results[0].time_sum[ans3[pointer2].pos]-results[0].time_sum[ans2[pointer1].pos]
                            -results[0].stopoverTimes[ans3[pointer2].pos-1]+(x-mid_time);//两站之间的时间
                        int price_sum=results[0].money_sum[ans3[pointer2].pos]-results[0].money_sum[ans2[pointer1].pos];
                        if(flag){
                            if(p){
                                if(!(price_sum<cost_s || (price_sum==cost_s && time_sum<time_s)))continue;
                            }else{
                                if(!(time_sum<time_s || (time_sum==time_s && price_sum<cost_s)))continue;
                            }
                        }
                        else flag=1;
                        Time x2=x+time_sum;
                        int seats=200000;
                        for(int k=ans2[pointer1].pos;k<ans3[pointer2].pos;k++){
                            seats=std::min(seats,results[0].ticket_left[no][k]);
                        }
                        ans=std::string(ans2[pointer1].ID.ID)+" "+std::string(result[0].stations[j])+" "+x.toString()+" -> "+
                            std::string(t)+" "+x2.toString()+" "+std::to_string(price_sum)+" "+std::to_string(seats);
                        cost_s=price_sum;
                        time_s=time_sum;
                        id_=ans2[pointer1].ID;//好像不用存
                    }
                }
                if(!flag)continue;
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
                for(int k=ans1[i].pos;k<j;k++){
                    seatsss=std::min(seatsss,result[0].ticket_left[no_][k]);
                }
                t1=std::string(ans1[i].ID.ID)+" "+std::string(s)+" "+xxx.toString()+" -> "+
                    std::string(result[0].stations[j])+" "+mid_time.toString()+" "+std::to_string(cost_sums)+" "+std::to_string(seatsss);
                t2=ans;
                time_summ=time_sums;
                cost_summ=cost_sums;
            }
            if(!flag2)continue;
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