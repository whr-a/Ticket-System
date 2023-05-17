#ifndef USER
#define USER
#include <string>
#include <cstring>
#include "map.hpp"
#include "database.hpp"
#include "utility.hpp"
class username
{
public:
    char un[21];
    username(){}
    username(char *obj){strcpy(un,obj);}
    friend bool operator < (const username &obj1,const username &obj2) {
        return strcmp(obj1.un,obj2.un)<0;
    }
    friend bool operator == (const username &obj1,const username &obj2) {
        return strcmp(obj1.un,obj2.un)==0;
    }
    friend bool operator != (const username &obj1,const username &obj2) {
        return strcmp(obj1.un,obj2.un)!=0;
    }
    username operator = (const username &obj){
        strcpy(un,obj.un);
        return *this;
    }
};
class account{
public:
    char username[21];
    char password[31];
    char name[20];
    char mailAddr[31];
    int privilege;
    friend bool operator < (const account &obj1,const account &obj2){
        return strcmp(obj1.username,obj2.username)<0;
    }
    account(){}
    account(char* u,char* p,char* n,char* m,int g){
        strcpy(username,u);
        strcpy(password,p);
        strcpy(name,n);
        strcpy(mailAddr,m);
        privilege=g;
    }
};

class user{
public:
    database<username,account> user_base;
    sjtu::map< username,int > login_set;
    user(){
        user_base.setfile("user.db");
    }
    void clear(){
        user_base.clear("user.db");
    }
    int adduser(char* c,char* u,char *p,char *n,char *m,int g){
        if(user_base.empty()){
            account val(u,p,n,m,10);
            user_base.insert(username(u),val);
            return 0;
        }
        if(user_base.find(u).empty() && login_set.find(username(c))!=login_set.end()){
            if(login_set[username(c)]<=g)return -1;
            account val(u,p,n,m,g);
            user_base.insert(username(u),val);
            return 0;
        }
        else return -1;
    }
    int login(char* u,char* p){
        sjtu::vector<account> ans=user_base.find(username(u));
        if(ans.empty())return -1;
        if(strcmp(ans[0].password,p)!=0)return -1;
        if(login_set.find(username(u))==login_set.end()){
            login_set.insert(sjtu::pair<username,int>(username(u),ans[0].privilege));
            return 0;
        }
        else return -1;
    }
    int logout(char *u){
        if(login_set.find(username(u))!=login_set.end()){
            login_set.erase(login_set.find(username(u)));
            return 0;
        }
        else return -1;
    }
    int query_profile(char *c,char *u){
        if(login_set.find(username(c))==login_set.end())return -1;
        sjtu::vector<account> ans=user_base.find(username(u));
        if(ans.empty())return -1;
        if(ans[0].privilege>login_set[username(c)])return -1;
        std::cout<<ans[0].username<<' '<<ans[0].name<<' '<<ans[0].mailAddr<<' '<<ans[0].privilege<<'\n';
        //if(strcmp(c,"lxy")==0)std::cout<<"李兴阳别开了"<<'\n';
        return 1;
    }
    int modify_profile(char *c,char *u,bool p_,char *p,bool n_,char *n,bool m_,char *m,bool g_,int g){
        if(login_set.find(username(c))==login_set.end())return -1;
        if(g_ && login_set[username(c)]<=g)return -1;
        sjtu::vector<account> ans=user_base.find(username(u));
        if(ans.empty())return -1;
        if(ans[0].privilege>login_set[username(c)])return -1;
        user_base.erase(username(u),ans[0]);
        if(p_)strcpy(ans[0].password,p);
        if(n_)strcpy(ans[0].name,n);
        if(m_)strcpy(ans[0].mailAddr,m);
        if(g_)ans[0].privilege=g;
        user_base.insert(username(u),ans[0]);
        if(g_){
            if(login_set.find(username(u))!=login_set.end())
                login_set[username(u)]=g;
        }
        std::cout<<ans[0].username<<' '<<ans[0].name<<' '<<ans[0].mailAddr<<' '<<ans[0].privilege<<'\n';
        return 1;
    }
};

#endif