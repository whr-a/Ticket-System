#ifndef USER
#define USER
#include <string>
#include <cstring>
#include "database_copy.hpp"
#include "utility.hpp"
class username
{
public:

    size_t un_hash;

    username(){un_hash=0;}
    username(const username &other){
        un_hash=other.un_hash;
    }
    username(const char *obj){un_hash=hash_func(obj);}
    username operator=(const username &obj){
        un_hash=obj.un_hash;
        return *this;
    }
    friend bool operator < (const username &obj1,const username &obj2) {
        return obj1.un_hash<obj2.un_hash;
    }
    friend bool operator <= (const username &obj1,const username &obj2) {
        return obj1.un_hash<=obj2.un_hash;
    }
    friend bool operator == (const username &obj1,const username &obj2) {
        return obj1.un_hash==obj2.un_hash;
    }
    friend bool operator != (const username &obj1,const username &obj2) {
        return obj1.un_hash!=obj2.un_hash;
    }
    // friend std::ostream & operator<<(std::ostream &os,const username &obj){
    //     os<<obj.un;
    //     return os;
    // }
    size_t hash_func(const char* obj){
        std::size_t hash = 0;
        for(int i=0;i<strlen(obj);i++){
            hash=(hash*31)+obj[i];
        }
        return hash;
    }
};
class MyUnorderedMap {
public:
    const static size_t TableSize=65536;
    struct Nodes {
        username key;
        int value;
        Nodes* next;
        Nodes(const username& k,int v)
            : key(k), value(v), next(nullptr) {}
    };
    // 构造函数
    MyUnorderedMap() {
        table = new Nodes*[TableSize]();
    }
    // 析构函数
    ~MyUnorderedMap() {
        clear();
        delete[] table;
    }

    // 插入键值对
    void insert(const username& key,int value) {
        int index = hashFunction(key);
        Nodes* newNode = new Nodes(key, value);
        newNode->next = table[index];
        table[index] = newNode;
    }

    // 删除键值对
    void erase(const username& key) {
        int index = hashFunction(key);
        Nodes* currNode = table[index];
        Nodes* prevNode = nullptr;

        while (currNode != nullptr) {
            if (currNode->key == key) {
                if (prevNode != nullptr)
                    prevNode->next = currNode->next;
                else
                    table[index] = currNode->next;

                delete currNode;
                break;
            }

            prevNode = currNode;
            currNode = currNode->next;
        }
    }

    // 获取值
    int& operator[](const username& key) {
        int index = hashFunction(key);
        Nodes* currNode = table[index];

        while (currNode != nullptr) {
            if (currNode->key == key)
                return currNode->value;
            currNode = currNode->next;
        }

        Nodes* newNode = new Nodes(key,0);
        newNode->next = table[index];
        table[index] = newNode;

        return newNode->value;
    }

    // 判断键是否存在
    bool contains(const username& key) const {
        int index = hashFunction(key);
        Nodes* currNode = table[index];

        while (currNode != nullptr) {
            if (currNode->key == key)
                return true;

            currNode = currNode->next;
        }

        return false;
    }

    // 获取键值对数量
    size_t size() const {
        size_t count = 0;

        for (int i = 0; i < TableSize; i++) {
            Nodes* currNode = table[i];
            while (currNode != nullptr) {
                count++;
                currNode = currNode->next;
            }
        }

        return count;
    }

    // 清空哈希表
    void clear() {
        for (int i = 0; i < TableSize; i++) {
            Nodes* currNode = table[i];
            while (currNode != nullptr) {
                Nodes* nextNode = currNode->next;
                delete currNode;
                currNode = nextNode;
            }
            table[i] = nullptr;
        }
    }


    // 哈希表节点定义
    

    Nodes** table;  // 哈希表数组

    // 哈希函数
    int hashFunction(const username& key) const {
        return key.un_hash%TableSize;
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
    account() { username[0] = 0; }
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
    MyUnorderedMap login_set;
    user(){
        user_base.setfile("user.db");
    }

    void clear(){
        user_base.clear("user.db");
        login_set.clear();
    }
    int adduser(char* c,char* u,char *p,char *n,char *m,int g){

        if(user_base.empty()){
            user_base.insert(username(u),account(u,p,n,m,10));
            return 0;
        }
        if(user_base.find(u).empty() && login_set.contains(username(c))){

            if(login_set[username(c)]<=g)return -1;
            
            user_base.insert(username(u),account(u,p,n,m,g));
            return 0;
        }
        else {
            // std::cout<<c<<std::endl;
            // std::cout<<login_set.hashFunction(username(c));

            return -1;
        }
    }
    int login(char* u,char* p){

        sjtu::vector<account> ans = user_base.find(username(u));
        if(ans.empty())return -1;
        
        if(strcmp(ans[0].password,p)!=0)return -1;
        if(!login_set.contains(username(u))){
            login_set.insert(username(u),ans[0].privilege);
            // std::cout<<u<<std::endl;
            // std::cout<<login_set.hashFunction(username(u));
            return 0;
        }
        else {
            //std::cout<<3<<std::endl;
            return -1;
        }
    }
    int logout(char *u){
        if(login_set.contains(username(u))){
            login_set.erase(username(u));
            return 0;
        }
        else return -1;
    }
    int query_profile(char *c,char *u){
        if(!login_set.contains(username(c)))return -1;
        sjtu::vector<account> ans=user_base.find(username(u));
        if(ans.empty())return -1;
        if(ans[0].privilege>login_set[username(c)])return -1;
        if(ans[0].privilege==login_set[username(c)] && strcmp(c,u)!=0)return -1;
        std::cout<<ans[0].username<<' '<<ans[0].name<<' '<<ans[0].mailAddr<<' '<<ans[0].privilege<<'\n';
        //if(strcmp(c,"lxy")==0)std::cout<<"李兴阳别开了"<<'\n';
        return 1;
    }
    int modify_profile(char *c,char *u,bool p_,char *p,bool n_,char *n,bool m_,char *m,bool g_,int g){
        if(!login_set.contains(username(c)))return -1;
        if(g_ && login_set[username(c)]<=g)return -1;
        sjtu::vector<account> ans=user_base.find(username(u));
        if(ans.empty())return -1;
        if(ans[0].privilege>login_set[username(c)])return -1;
        if(ans[0].privilege==login_set[username(c)] && strcmp(c,u)!=0)return -1;
        user_base.erase(username(u),ans[0]);
        if(p_)strcpy(ans[0].password,p);
        if(n_)strcpy(ans[0].name,n);
        if(m_)strcpy(ans[0].mailAddr,m);
        if(g_)ans[0].privilege=g;
        user_base.insert(username(u),ans[0]);
        if(g_){
            if(login_set.contains(username(u)))
                login_set[username(u)]=g;
        }
        std::cout<<ans[0].username<<' '<<ans[0].name<<' '<<ans[0].mailAddr<<' '<<ans[0].privilege<<'\n';
        return 1;
    }
};

#endif