#ifndef TICKET_LEFT
#define TICKET_LEFT
#include <iostream>
#include <cstring>
#include <string>
#include "database.hpp"
class train_f
{
public:
    char trainID[21];
    int date;//第几天的车
    train_f(){trainID[0]=0;}
    train_f(char* trainID_,int date_):date(date_){
        strcpy(trainID,trainID_);
    }
    train_f(const train_f &other){
        strcpy(trainID,other.trainID);
        date=other.date;
    }
    friend bool operator<(const train_f &obj1,const train_f &obj2){
        int t=strcmp(obj1.trainID,obj2.trainID);
        if(t<0)return true;
        else if(t==0 && obj1.date<obj2.date)return true;
        else return false;
    }
    friend bool operator<=(const train_f &obj1,const train_f &obj2){
        int t=strcmp(obj1.trainID,obj2.trainID);
        if(t<0)return true;
        else if(t==0 && obj1.date<=obj2.date)return true;
        else return false;
    }
    friend bool operator==(const train_f &obj1,const train_f &obj2){
        return (!(obj1<obj2)) && (!(obj2<obj1));
    }
    friend bool operator!=(const train_f &obj1,const train_f &obj2){
        return !(obj1==obj2);
    }
};
class ticket_left
{
public:
    int ticket[101];
    ticket_left(){}
    ticket_left(const ticket_left &other){
        for(int i=0;i<101;i++)ticket[i]=other.ticket[i];
    }
    ticket_left(int *t,int num){
        for(int i=0;i<num;i++)ticket[i]=t[i];
    }
    friend bool operator<(const ticket_left &obj1,const ticket_left &obj2){
        return false;
    }
};
class ticket_base
{
public:
    database<train_f,ticket_left> ticket_base_;
    ticket_base(){
        ticket_base_.setfile("ticket.db");
    }
    void add(const train_f &i1,const ticket_left &i2){
        ticket_base_.insert(i1,i2);
    }
    ticket_left query(const train_f &i1){
        return ticket_base_.find(i1)[0];
    }
    void erase(const train_f &i1,const ticket_left &i2){
        ticket_base_.erase(i1,i2);
    }
    void modify(const train_f &i1,const ticket_left &i2){
        ticket_base_.erase(i1,ticket_base_.find(i1)[0]);
        ticket_base_.insert(i1,i2);
    }
    void clear(){
        ticket_base_.clear("ticket.db");
    }
};
#endif