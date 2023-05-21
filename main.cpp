#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <cmath>
#include <fstream>
#include "database_copy.hpp"
#include "ticket_left.hpp"
#include "vector.hpp"
#include "Tokenscanner.hpp"
#include "users.hpp"
#include "utility.hpp"
#include "ticket.hpp"
#include "trains.hpp"
bool quit_=0;
void processLine(std::string &line,user &users,train &trains,ticket &tickets,ticket_base &ticket_lefts);
int main ()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    user users;
    train trains;
    ticket tickets;
    ticket_base ticket_lefts;
    //std::cout<<sizeof(char)<<std::endl;
    while (true) {
        std::string input;
        if(!getline(std::cin, input))return 0;
        processLine(input,users,trains,tickets,ticket_lefts);
        if(quit_)break;
        //std::cout.flush();
    }
}
void processLine(std::string &line,user &users,train &trains,ticket &tickets,ticket_base &ticket_lefts){
    Tokenscanner scanner;
    scanner.setInput(line);
    std::string s=scanner.nextToken();
    if(s.empty())return;
    int time=scanner.check_num(s.substr(1,s.size()-2));
    // if(time==750566){
    //     quit_=1;
    //     return;
    // }
    s=scanner.nextToken();
    if(s=="add_user"){
        char c[21],u[21],p[31],n[20],m[31];int g;
        while(scanner.haveMoreTokens()){
            s=scanner.nextToken();
            switch(s[1]){
                case 'c':
                    s=scanner.nextToken();
                    strcpy(c,s.c_str());
                    break;
                case 'u':
                    s=scanner.nextToken();
                    strcpy(u,s.c_str());
                    break;
                case 'p':
                    s=scanner.nextToken();
                    strcpy(p,s.c_str());
                    break;
                case 'n':
                    s=scanner.nextToken();
                    strcpy(n,s.c_str());
                    break;
                case 'm':
                    s=scanner.nextToken();
                    strcpy(m,s.c_str());
                    break;
                case 'g':
                    s=scanner.nextToken();
                    g=scanner.check_num(s);
                    break;
            }
        }
        std::cout<<'['<<time<<"] "<<users.adduser(c,u,p,n,m,g)<<'\n';
    }else if(s=="login"){
        char u[21],p[31];
        while(scanner.haveMoreTokens()){
            s=scanner.nextToken();
            switch(s[1]){
                case 'u':
                    s=scanner.nextToken();
                    strcpy(u,s.c_str());
                    break;
                case 'p':
                    s=scanner.nextToken();
                    strcpy(p,s.c_str());
                    break;
            }
        }
        std::cout<<'['<<time<<"] "<<users.login(u,p)<<'\n';
    }else if(s=="logout"){
        char u[21];
        s=scanner.nextToken();
        s=scanner.nextToken();
        strcpy(u,s.c_str());
        std::cout<<'['<<time<<"] "<<users.logout(u)<<'\n';
    }else if(s=="query_profile"){
        char c[21],u[31];
        while(scanner.haveMoreTokens()){
            s=scanner.nextToken();
            switch(s[1]){
                case 'c':
                    s=scanner.nextToken();
                    strcpy(c,s.c_str());
                    break;
                case 'u':
                    s=scanner.nextToken();
                    strcpy(u,s.c_str());
                    break;
            }
        }
        std::cout<<'['<<time<<"] ";
        if(users.query_profile(c,u)==-1)std::cout<<-1<<'\n';
    }else if(s=="modify_profile"){
        char c[21],u[21],p[31],n[20],m[31];int g;
        bool p_=0,n_=0,m_=0,g_=0;
        while(scanner.haveMoreTokens()){
            s=scanner.nextToken();
            switch(s[1]){
                case 'c':
                    s=scanner.nextToken();
                    strcpy(c,s.c_str());
                    break;
                case 'u':
                    s=scanner.nextToken();
                    strcpy(u,s.c_str());
                    break;
                case 'p':
                    p_=1;
                    s=scanner.nextToken();
                    strcpy(p,s.c_str());
                    break;
                case 'n':
                    n_=1;
                    s=scanner.nextToken();
                    strcpy(n,s.c_str());
                    break;
                case 'm':
                    m_=1;
                    s=scanner.nextToken();
                    strcpy(m,s.c_str());
                    break;
                case 'g':
                    g_=1;
                    s=scanner.nextToken();
                    g=scanner.check_num(s);
                    break;
            }
        }
        std::cout<<'['<<time<<"] ";
        if(users.modify_profile(c,u,p_,p,n_,n,m_,m,g_,g)==-1)std::cout<<-1<<'\n';
    }else if(s=="add_train"){
        char i[21],y;
        std::string st[101];
        int n,m,p[101],t[101],o[101];
        sjtu::daytime x;
        sjtu::monthtime d[2];
        while(scanner.haveMoreTokens()){
            s=scanner.nextToken();
            int cnt=-1;
            std::string temp;
            switch(s[1]){
                case 'i':
                    s=scanner.nextToken();
                    strcpy(i,s.c_str());
                    break;
                case 'n':
                    n=scanner.check_num(scanner.nextToken());
                    break;
                case 'm':
                    m=scanner.check_num(scanner.nextToken());
                    break;
                case 's':
                    s=scanner.nextToken();
                    for(int j=0;j<s.size();j++){
                        if(s[j]!='|')temp+=s[j];
                        else{
                            ++cnt;
                            st[cnt]=temp;
                            temp.clear();
                        }
                    }
                    ++cnt;
                    st[cnt]=temp;
                    break;
                case 'p':
                    s=scanner.nextToken();
                    for(int j=0;j<s.size();j++){
                        if(s[j]!='|')temp+=s[j];
                        else{
                            ++cnt;
                            p[cnt]=scanner.check_num(temp);
                            temp.clear();
                        }
                    }
                    ++cnt;
                    p[cnt]=scanner.check_num(temp);
                    break;
                case 'x':
                    s=scanner.nextToken();
                    x=sjtu::daytime(scanner.check_num(s.substr(0,2)),scanner.check_num(s.substr(3,2)));
                    break;
                case 't':
                    s=scanner.nextToken();
                    for(int j=0;j<s.size();j++){
                        if(s[j]!='|')temp+=s[j];
                        else{
                            ++cnt;
                            t[cnt]=scanner.check_num(temp);
                            temp.clear();
                        }
                    }
                    ++cnt;
                    t[cnt]=scanner.check_num(temp);
                    break;
                case 'o':
                    s=scanner.nextToken();
                    for(int j=0;j<s.size();j++){
                        if(s[j]!='|')temp+=s[j];
                        else{
                            ++cnt;
                            o[cnt]=scanner.check_num(temp);
                            temp.clear();
                        }
                    }
                    ++cnt;
                    o[cnt]=scanner.check_num(temp);
                    break;
                case 'd':
                    s=scanner.nextToken();
                    d[0]=sjtu::monthtime(scanner.check_num(s.substr(0,2)),scanner.check_num(s.substr(3,2)));
                    d[1]=sjtu::monthtime(scanner.check_num(s.substr(6,2)),scanner.check_num(s.substr(9,2)));
                    break;
                case 'y':
                    s=scanner.nextToken();
                    y=s[0];
                    break;
            }
        }
        std::cout<<'['<<time<<"] "<<trains.addtrain(i,n,m,st,p,x,t,o,d,y,ticket_lefts)<<'\n';
    }else if(s=="delete_train"){
        char i[21];
        s=scanner.nextToken();
        s=scanner.nextToken();
        strcpy(i,s.c_str());
        std::cout<<'['<<time<<"] "<<trains.delete_train(i,ticket_lefts)<<'\n';
    }else if(s=="release_train"){
        char i[21];
        s=scanner.nextToken();
        s=scanner.nextToken();
        strcpy(i,s.c_str());
        std::cout<<'['<<time<<"] "<<trains.release_train(i,ticket_lefts)<<'\n';
    }else if(s=="query_train"){
        char i[21];
        int m,d;
        while(scanner.haveMoreTokens()){
            s=scanner.nextToken();
            switch(s[1]){
                case 'i':
                    s=scanner.nextToken();
                    strcpy(i,s.c_str());
                    break;
                case 'd':
                    s=scanner.nextToken();
                    m=scanner.check_num(s.substr(0,2));
                    d=scanner.check_num(s.substr(3,2));
                    break;
            }
        }
        std::cout<<'['<<time<<"] ";
        if(trains.query_train(i,m,d,ticket_lefts)==-1)std::cout<<-1<<'\n';
    }else if(s=="query_ticket"){
        char st[33],to[33];
        int m,d;
        bool p=0;
        while(scanner.haveMoreTokens()){
            s=scanner.nextToken();
            switch(s[1]){
                case 's':
                    strcpy(st,scanner.nextToken().c_str());
                    break;
                case 't':
                    strcpy(to,scanner.nextToken().c_str());
                    break;
                case 'd':
                    s=scanner.nextToken();
                    m=scanner.check_num(s.substr(0,2));
                    d=scanner.check_num(s.substr(3,2));
                    break;
                case 'p':
                    s=scanner.nextToken();
                    if(s=="cost")p=1;
                    break;  
            }
        }
        std::cout<<'['<<time<<"] ";
        trains.query_ticket(st,to,m,d,p,ticket_lefts);
    }else if(s=="query_transfer"){
        char st[33],to[33];
        int m,d;
        bool p=0;
        while(scanner.haveMoreTokens()){
            s=scanner.nextToken();
            switch(s[1]){
                case 's':
                    strcpy(st,scanner.nextToken().c_str());
                    break;
                case 't':
                    strcpy(to,scanner.nextToken().c_str());
                    break;
                case 'd':
                    s=scanner.nextToken();
                    m=scanner.check_num(s.substr(0,2));
                    d=scanner.check_num(s.substr(3,2));
                    break;
                case 'p':
                    s=scanner.nextToken();
                    if(s=="cost")p=1;
                    break;  
            }
        }
        std::cout<<'['<<time<<"] ";
        if(trains.query_transfer(st,to,m,d,p,ticket_lefts)==0)std::cout<<0<<'\n';
    }else if(s=="buy_ticket"){
        char u[21],i[21],f[33],t[33];
        int m,d,n;
        bool q=0;
        while(scanner.haveMoreTokens()){
            s=scanner.nextToken();
            switch(s[1]){
                case 'u':
                    strcpy(u,scanner.nextToken().c_str());
                    break;
                case 'i':
                    strcpy(i,scanner.nextToken().c_str());
                    break;
                case 'd':
                    s=scanner.nextToken();
                    m=scanner.check_num(s.substr(0,2));
                    d=scanner.check_num(s.substr(3,2));
                    break;
                case 'n':
                    n=scanner.check_num(scanner.nextToken());
                    break;
                case 'f':
                    strcpy(f,scanner.nextToken().c_str());
                    break;
                case 't':
                    strcpy(t,scanner.nextToken().c_str());
                    break;
                case 'q':
                    s=scanner.nextToken();
                    if(s=="true")q=1;
                    break;
            }
        }
        std::cout<<'['<<time<<"] ";
        int tttt=tickets.buy_ticket(u,i,m,d,n,f,t,q,trains,users,time,ticket_lefts);
        if(tttt==-1)std::cout<<-1<<'\n';
        else if(tttt==-2)std::cout<<"queue\n";
        else std::cout<<tttt<<'\n';
    }else if(s=="query_order"){
        char u[21];
        scanner.nextToken();
        strcpy(u,scanner.nextToken().c_str());
        std::cout<<'['<<time<<"] ";
        if(tickets.query_order(u,trains,users)==-1)std::cout<<-1<<'\n';
    }else if(s=="refund_ticket"){
        char u[21];
        int n=1;
        while(scanner.haveMoreTokens()){
            s=scanner.nextToken();
            switch(s[1]){
                case 'u':
                    strcpy(u,scanner.nextToken().c_str());
                    break;
                case 'n':
                    n=scanner.check_num(scanner.nextToken());
                    break;
            }
        }
        std::cout<<'['<<time<<"] "<<tickets.refund_ticket(u,n,trains,users,ticket_lefts)<<'\n';
    }else if(s=="clean"){
        users.clear();
        trains.clear();
        tickets.clear();
        ticket_lefts.clear();
        std::cout<<'['<<time<<"] "<<0<<'\n';
    }else if(s=="exit"){
        quit_=1;
        std::cout<<'['<<time<<"] "<<"bye"<<'\n';
    }
}