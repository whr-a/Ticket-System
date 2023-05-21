#ifndef DATABASE
#define DATABASE

#include <exception>
#include <iostream>
#include <cstring>
#include <string>
#include <cstdio>
#include <cmath>
#include <fstream>
#include "utility.hpp"
#include "vector.hpp"

template<class keys,class T>
class database
{
private:
    class data{
    public:
        keys key;
        T value;
        data(){}
        data(const keys &obj,const T &value_){
            key=obj;
            value=value_;
        }
        friend bool operator < (const data &obj1,const data &obj2){
            if(obj1.key<obj2.key)return true;
            if(obj1.key == obj2.key && obj1.value < obj2.value)return true;
            return false;
        }
        friend bool operator > (const data &obj1,const data &obj2){
            return obj2<obj1;
        }
        friend bool operator == (const data &obj1,const data &obj2){
            return (!(obj1<obj2))&&(!(obj2<obj1));
        }
        friend bool operator >= (const data &obj1,const data &obj2){
            return !(obj1<obj2);
        }
        friend bool operator <= (const data &obj1,const data &obj2){
            return !(obj2<obj1);
        }
    };
    static const int size_of_block=20;
    std::fstream opfile;

    class start
    {
    public:
        int pos_of_root;
        int num_of_block;
        start(){
            pos_of_root=1;
            num_of_block=1;
        }
        start(int pos_of_root_,int num_of_block_)
            :pos_of_root(pos_of_root_),num_of_block(num_of_block_){}
    };
    start head;
    enum nodetype{index,leaf};
    class node
    {
    public:
        nodetype type;
        int pos_of_fa;
        int now_num;
        int edge[size_of_block+1];
        data value[size_of_block];
        int front_pos,back_pos;
        node(){
            type=leaf;
            now_num=0;
            pos_of_fa=0;
            front_pos=0;
            back_pos=0;
            memset(edge,0,sizeof(edge));
        }
    };
    void writenode(node *obj,int num){
        opfile.seekp(sizeof(start)+(num-1)*sizeof(node));
        opfile.write(reinterpret_cast<char*>(obj),sizeof(node));
    }
    void writenode_(node &obj,int num){
        opfile.seekp(sizeof(start)+(num-1)*sizeof(node));
        opfile.write(reinterpret_cast<char*>(&obj),sizeof(node));
    }
    //LinkedHashMap节点定义
    struct Node {
        int key;
        node* value;
        Node* prev;
        Node* next;

        Node()
            : prev(nullptr), next(nullptr) {}

        Node(int k,node* v)
            : key(k), value(v), prev(nullptr), next(nullptr) {}
    };
    // 哈希表节点定义
    struct Nodes {
        int key;
        Node* value;
        Nodes* next;

        Nodes(const int& k,Node* v)
            : key(k), value(v), next(nullptr) {}
    };
    class LinkedHashMap {
    public:
        class MyUnorderedMap {
        public:
            // 构造函数
            const static size_t TableSize=128;
            MyUnorderedMap() {
                table = new Nodes*[TableSize]();
            }
            // 析构函数
            ~MyUnorderedMap() {
                clear();
                delete[] table;
            }

            // 插入键值对
            void insert(const int& key, const node*& value) {
                int index = hashFunction(key);
                Nodes* newNode = new Nodes(key, value);
                newNode->next = table[index];
                table[index] = newNode;
            }

            // 删除键值对
            void erase(const int& key) {
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
            Node*& operator[](const int& key) {
                int index = hashFunction(key);
                Nodes* currNode = table[index];

                while (currNode != nullptr) {
                    if (currNode->key == key)
                        return currNode->value;

                    currNode = currNode->next;
                }

                Nodes* newNode = new Nodes(key,nullptr);
                newNode->next = table[index];
                table[index] = newNode;

                return newNode->value;
            }

            // 判断键是否存在
            bool contains(const int& key) const {
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
            int hashFunction(const int& key) const {
                return std::hash<int>()(key) % TableSize;
            }
        };
        // 构造函数
        LinkedHashMap() {
            head = new Node();
            tail = new Node();
            head->next = tail;
            tail->prev = head;
        }

        // 析构函数
        ~LinkedHashMap() {}

        // 插入键值对
        Node* insert(const int& key,node* value) {
            if (map.contains(key)) {
                // 键已存在，更新值并将节点移到链表头部
                Node* node_ = map[key];
                node_->value = value;
                moveToHead(node_);
                return nullptr;
            } else {
                // 键不存在，创建新节点并插入到链表头部
                Node* node_ = new Node(key, value);
                map[key] = node_;
                insertToHead(node_);
                
                // 检查容量是否超出限制
                if (map.size() > capacity) {
                    // 超出限制，移除链表尾部的节点
                    Node* tailNode = removeTail();
                    return tailNode;
                    // map.erase(tailNode->key);
                    // delete tailNode->value;
                    // delete tailNode;
                }
                return nullptr;
            }
        }

        // 删除键值对
        void erase(const int& key) {
            if (map.contains(key)) {
                Node* node = map[key];
                removeNode(node);
                map.erase(key);
                delete node;
            }
        }

        // 获取值
        node* operator[](const int& key) {//危险，要先contains一下才能用！！！
            Node* node = map[key];
            moveToHead(node);
            return node->value;
        }

        // 判断键是否存在
        bool contains(const int& key) const {
            return map.contains(key);
        }

        // 获取键值对数量
        size_t size() const {
            return map.size();
        }

        // 清空链表和哈希表
        void clear() {
            for (int i = 0; i < map.TableSize; i++) {
                Nodes* currNode = map.table[i];
                while (currNode != nullptr) {
                    Nodes* nextNode = currNode->next;
                    delete currNode->value->value;
                    delete currNode->value;
                    delete currNode;
                    currNode = nextNode;
                }
                map.table[i] = nullptr;
            }
            head->next = tail;
            tail->prev = head;
        }

        // 链表节点定义
    
        Node* head;  // 链表头节点
        Node* tail;  // 链表尾节点
        MyUnorderedMap map;  // 哈希表，用于快速查找节点
        static const size_t capacity = 128;  // 容量限制，可以根据需求进行调整

        // 将节点插入到链表头部
        void insertToHead(Node* node) {
            node->next = head->next;
            node->prev = head;
            head->next->prev = node;
            head->next = node;
        }

        // 将节点移动到链表头部
        void moveToHead(Node* node) {
            removeNode(node);
            insertToHead(node);
        }

        // 移除节点
        void removeNode(Node* node) {
            node->prev->next = node->next;
            node->next->prev = node->prev;
        }

        // 移除链表尾部的节点
        Node* removeTail() {
            Node* tailNode = tail->prev;
            removeNode(tailNode);
            return tailNode;
        }
    };
    LinkedHashMap map;
    //int cnt=0;
    void getstart(start &st){
        opfile.seekg(0);
        opfile.read(reinterpret_cast<char*>(&st),sizeof(start));
    }
    void writestart(start &st){
        opfile.seekp(0);
        opfile.write(reinterpret_cast<char*>(&st),sizeof(start));
    }
    void getnode(node *&obj,int num) {
        if(!map.contains(num)){
            node* newNode = new node();
            Node* x=map.insert(num,newNode);
            if(x!=nullptr){
                writenode(x->value,x->key);
                map.map.erase(x->key);
                delete x->value;
                delete x;
            }
            opfile.seekg(sizeof(start) + (num-1)*sizeof(node));
            opfile.read(reinterpret_cast<char*>(newNode),sizeof(node));
            obj=newNode;
        }
        else obj=map[num];
    }
    
    int finds(const keys &key,int num){
        node* temp;
        getnode(temp,num);
        if(temp->type == leaf)return num;
        int l = 0, r = temp->now_num - 1, mid;
        while (l <= r) {
            int mid = (l + r) >> 1;
            if (temp->value[mid].key < key)
                l = mid + 1;
            else
                r = mid - 1;
        }
        return finds(key,temp->edge[r+1]);
    }
    int find_pos(const data &obj,int num){
        node* temp;
        getnode(temp,num);
        if(temp->type==leaf)return num;
        else{
            int t=sjtu::upper_bound(temp->value,temp->value+temp->now_num,obj)-temp->value;
            //if(temp->edge[t]>head.num_of_block)std::cout<<114514<<std::endl;
            return find_pos(obj,temp->edge[t]);
        }
    }
    void updatefather(int pos,int fa_pos){
        node* temp;
        getnode(temp,pos);
        temp->pos_of_fa=fa_pos;
    }
    void updateleft(int pos,int left){
        if(pos==0)return;
        node* temp;
        getnode(temp,pos);
        temp->back_pos=left;
    }
    void updateright(int pos,int right){
        if(pos==0)return;
        node* temp;
        getnode(temp,pos);
        temp->front_pos=right;
    }
    void flashindex(int fa_pos,int son_pos,const data &obj){
        node* temp;
        getnode(temp,fa_pos);
        int t=sjtu::upper_bound(temp->value,temp->value+temp->now_num,obj)-temp->value;
        for(int i=temp->now_num;i>t;i--){
            temp->value[i]=temp->value[i-1];
            temp->edge[i+1]=temp->edge[i];
        }//proofed
        temp->value[t]=obj;
        temp->edge[t+1]=son_pos;
        temp->now_num++;
        if(temp->now_num==size_of_block){
            node temp2;
            int mid=size_of_block/2;
            temp2.now_num=mid-1;
            for(int i=mid+1;i<size_of_block;i++){
                temp2.value[i-mid-1]=temp->value[i];
                temp2.edge[i-mid-1]=temp->edge[i];
                updatefather(temp->edge[i],head.num_of_block+1);
            }
            temp2.edge[mid-1]=temp->edge[size_of_block];
            updatefather(temp->edge[size_of_block],head.num_of_block+1);
            temp->now_num=mid;
            temp2.type=index;
            if(temp->pos_of_fa==0){
                temp->pos_of_fa=head.num_of_block+2;
                temp2.pos_of_fa=head.num_of_block+2;
                writenode_(temp2,head.num_of_block+1);
                head.num_of_block++;
                node temp3;
                temp3.edge[0]=fa_pos;
                temp3.edge[1]=head.num_of_block;
                temp3.value[0]=temp->value[mid];
                temp3.now_num=1;
                temp3.type=index;
                writenode_(temp3,head.num_of_block+1);
                head.num_of_block++;
                head.pos_of_root=head.num_of_block;
            }
            else{
                temp2.pos_of_fa=temp->pos_of_fa;
                writenode_(temp2,head.num_of_block+1);
                head.num_of_block++;
                flashindex(temp->pos_of_fa,head.num_of_block,temp->value[mid]);
            }
        }
    }
    void devideleaf(int pos){
        node* temp1;
        getnode(temp1,pos);
        node temp2;
        temp2.back_pos=pos;
        temp2.front_pos=temp1->front_pos;
        updateleft(temp1->front_pos,head.num_of_block+1);
        temp1->front_pos=head.num_of_block+1;
        for(int i=0;i<size_of_block/2;i++)temp2.value[i]=temp1->value[size_of_block/2+i];
        temp2.now_num=size_of_block/2;
        temp1->now_num=size_of_block/2;
        if(temp1->pos_of_fa==0){
            temp1->pos_of_fa=head.num_of_block+2;
            temp2.pos_of_fa=head.num_of_block+2;
            writenode_(temp2,head.num_of_block+1);
            head.num_of_block++;
            node temp3;
            temp3.now_num=1;
            temp3.edge[0]=pos;
            temp3.edge[1]=head.num_of_block;
            temp3.value[0]=temp2.value[0];
            temp3.type=index;
            writenode_(temp3,head.num_of_block+1);
            head.num_of_block++;
            head.pos_of_root=head.num_of_block;
        }
        else{
            temp2.pos_of_fa=temp1->pos_of_fa;
            writenode_(temp2,head.num_of_block+1);
            head.num_of_block++;
            flashindex(temp1->pos_of_fa,head.num_of_block,temp2.value[0]);
        }
    }
    bool judge(int pos,int fa,node*& temp){
        if(pos==0)return false;
        getnode(temp,pos);
        if(temp->pos_of_fa!=fa)return false;
        return true;
    }
    void merge(int pos,node* obj1,node* obj2,node* fa,int tx){
        obj1->value[obj1->now_num]=fa->value[tx];
        obj1->now_num++;
        for(int i=0;i<obj2->now_num;i++){
            obj1->value[obj1->now_num+i]=obj2->value[i];
            obj1->edge[obj1->now_num+i]=obj2->edge[i];
            node temp;
            updatefather(obj2->edge[i],pos);
        }
        obj1->now_num+=obj2->now_num;
        obj1->edge[obj1->now_num]=obj2->edge[obj2->now_num];
        updatefather(obj2->edge[obj2->now_num],pos);
    }
    void balanceindex(int pos,data delindex){
        node* temp;
        getnode(temp,pos);
        if(pos==head.pos_of_root && temp->now_num==1){
            node* son;
            getnode(son,temp->edge[0]);
            son->pos_of_fa=0;
            head.pos_of_root=temp->edge[0];
            return;
        }
        int t=sjtu::upper_bound(temp->value,temp->value+temp->now_num,delindex)-temp->value;
        for(int i=t-1;i<temp->now_num-1;i++)temp->value[i]=temp->value[i+1];
        for(int i=t;i<temp->now_num;i++)temp->edge[i]=temp->edge[i+1];
        temp->now_num--;
        if(pos==head.pos_of_root)return;
        
        if(temp->now_num<size_of_block/2-1){
            node* fa;
            getnode(fa,temp->pos_of_fa);
            int tx=sjtu::upper_bound(fa->value,fa->value+fa->now_num,temp->value[0])-fa->value;
            if(tx!=fa->now_num){
                node* rightbro;
                getnode(rightbro,fa->edge[tx+1]);
                if(rightbro->now_num>=size_of_block/2){
                    temp->edge[size_of_block/2-1]=rightbro->edge[0];
                    updatefather(rightbro->edge[0],pos);
                    temp->value[size_of_block/2-2]=fa->value[tx];
                    fa->value[tx]=rightbro->value[0];
                    for(int i=0;i<rightbro->now_num-1;i++){
                        rightbro->value[i]=rightbro->value[i+1];
                        rightbro->edge[i]=rightbro->edge[i+1];
                    }
                    rightbro->edge[rightbro->now_num-1]=rightbro->edge[rightbro->now_num];
                    rightbro->now_num--;
                    temp->now_num++;
                }
                else{
                    merge(pos,temp,rightbro,fa,tx);
                    balanceindex(temp->pos_of_fa,fa->value[tx]);
                }
            }
            else{
                node *leftbro;
                getnode(leftbro,fa->edge[tx-1]);
                if(leftbro->now_num>=size_of_block/2){
                    for(int i=temp->now_num;i>0;i--){
                        temp->value[i]=temp->value[i-1];
                        temp->edge[i+1]=temp->edge[i];
                    }
                    temp->edge[1]=temp->edge[0];
                    temp->edge[0]=leftbro->edge[leftbro->now_num];
                    temp->value[0]=fa->value[fa->now_num-1];
                    updatefather(temp->edge[0],pos);
                    temp->now_num++;
                    leftbro->now_num--;
                    fa->value[tx-1]=leftbro->value[leftbro->now_num];
                }
                else{
                    merge(fa->edge[tx-1],leftbro,temp,fa,tx-1);
                    balanceindex(temp->pos_of_fa,fa->value[tx-1]);
                }
            }
        }
    }
    void balanceleaf(int pos){//平衡叶子节点，现在大小小于size_of_block-1
        node *temp,*temp_back=nullptr,*temp_front=nullptr;//前兄弟和后兄弟节点
        getnode(temp,pos);
        if(!temp->back_pos && !temp->front_pos)return;//前后都没有节点，说明是孤寡，只可能是根节点
        if(!judge(temp->back_pos,temp->pos_of_fa,temp_back)){//后面没有节点或者后面的节点并非兄弟节点
            getnode(temp_front,temp->front_pos);//获得前面一个节点
            if(temp_front->now_num>size_of_block/2-1){//可以借
                temp->value[size_of_block/2-2]=temp_front->value[0];
                for(int i=0;i<temp_front->now_num-1;i++)temp_front->value[i]=temp_front->value[i+1];
                temp_front->now_num--;
                temp->now_num++;
                node* temp_fa;
                getnode(temp_fa,temp->pos_of_fa);
                int t=sjtu::upper_bound(temp_fa->value,temp_fa->value+temp_fa->now_num,
                                temp->value[size_of_block/2-2])-temp_fa->value;
                temp_fa->value[t-1]=temp_front->value[0];
            }
            else{//不能借，要合并
                for(int i=size_of_block/2-2;i<size_of_block-3;i++)
                    temp->value[i]=temp_front->value[i-(size_of_block/2-2)];
                temp->front_pos=temp_front->front_pos;
                updateleft(temp->front_pos,pos);
                temp->now_num=size_of_block-3;
                balanceindex(temp->pos_of_fa,temp_front->value[0]);
            }
            return;
        }
        if(!judge(temp->front_pos,temp->pos_of_fa,temp_front)){//前面一个节点不存在或非兄弟
            if(temp_back->now_num>size_of_block/2-1){//借
                for(int i=temp->now_num;i>0;i--)temp->value[i]=temp->value[i-1];
                temp->value[0]=temp_back->value[temp_back->now_num-1];
                temp_back->now_num--;
                temp->now_num++;
                node* temp_fa;
                getnode(temp_fa,temp->pos_of_fa);
                int t=sjtu::upper_bound(temp_fa->value,temp_fa->value+temp_fa->now_num,
                                 temp->value[0])-temp_fa->value;
                temp_fa->value[t]=temp->value[0];
            }
            else{//合并
                for(int i=size_of_block/2-1;i<size_of_block-3;i++)
                    temp_back->value[i]=temp->value[i-size_of_block/2+1];
                temp_back->front_pos=temp->front_pos;
                updateleft(temp_back->front_pos,temp->back_pos);
                temp_back->now_num=size_of_block-3;
                balanceindex(temp_back->pos_of_fa,temp->value[0]);
            }
            return;
        }
        //走到这里说明左右都有且是兄弟
        // getnode(temp_back,temp->back_pos);
        // getnode(temp_front,temp->front_pos);

        if(temp_back->now_num>size_of_block/2-1){//找前面借
            for(int i=temp->now_num;i>0;i--)temp->value[i]=temp->value[i-1];
            temp->value[0]=temp_back->value[temp_back->now_num-1];
            temp_back->now_num--;
            temp->now_num++;
            node* temp_fa;
            getnode(temp_fa,temp->pos_of_fa);
            int t=sjtu::upper_bound(temp_fa->value,temp_fa->value+temp_fa->now_num,
                                temp->value[0])-temp_fa->value;
            temp_fa->value[t]=temp->value[0];
            return;
        }
        if(temp_front->now_num>size_of_block/2-1){//找后面借
            temp->value[size_of_block/2-2]=temp_front->value[0];
            for(int i=0;i<temp_front->now_num-1;i++)temp_front->value[i]=temp_front->value[i+1];
            temp_front->now_num--;
            temp->now_num++;
            node* temp_fa;
            getnode(temp_fa,temp->pos_of_fa);
            int t=sjtu::upper_bound(temp_fa->value,temp_fa->value+temp_fa->now_num,
                            temp->value[size_of_block/2-2])-temp_fa->value;
            temp_fa->value[t-1]=temp_front->value[0];//修改
            return;
        }
        //都不能借，只好找前面合并
        for(int i=size_of_block/2-2;i<size_of_block-3;i++)
            temp->value[i]=temp_front->value[i-(size_of_block/2-2)];
        temp->front_pos=temp_front->front_pos;
        updateleft(temp->front_pos,pos);
        temp->now_num=size_of_block-3;
        balanceindex(temp->pos_of_fa,temp_front->value[0]);
    }
    void freshleft(int pos,const data &obj){
        if(pos==0)return;
        node* temp;
        getnode(temp,pos);
        int t=sjtu::upper_bound(temp->value,temp->value+temp->now_num,obj)-temp->value;
        if(t!=0){
            temp->value[t-1]=obj;
        }
        else freshleft(temp->pos_of_fa,obj);
    }
public:
    database(){}
    database(std::string name){
        std::ifstream in;
        in.open(name);
        if(!in){
            std::ofstream outfile(name);
            outfile.seekp(0);
            start t1;
            outfile.write(reinterpret_cast<char*>(&t1),sizeof(start));
            outfile.seekp(sizeof(start));
            node t2;
            outfile.write(reinterpret_cast<char*>(&t2),sizeof(node));
        }
        opfile.open(name);
        getstart(head);
    }
    void clear(std::string name){
        std::ofstream file(name, std::ios::trunc);
        file.close();
        map.clear();
        std::ifstream in;
        in.open(name);
        if(!in){
            std::ofstream outfile(name);
            outfile.seekp(0);
            start t1;
            outfile.write(reinterpret_cast<char*>(&t1),sizeof(start));
            outfile.seekp(sizeof(start));
            node t2;
            outfile.write(reinterpret_cast<char*>(&t2),sizeof(node));
        }
        opfile.open(name);
        getstart(head);
    }
    void setfile(std::string name){
        std::ifstream in;
        in.open(name);
        if(!in){
            std::ofstream outfile(name);
            outfile.seekp(0);
            start t1;
            outfile.write(reinterpret_cast<char*>(&t1),sizeof(start));
            outfile.seekp(sizeof(start));
            node t2;
            outfile.write(reinterpret_cast<char*>(&t2),sizeof(node));
        }
        opfile.open(name);
        getstart(head);
    }
    ~database(){
        writestart(head);
        for (int i = 0; i < map.map.TableSize; i++) {
            Nodes* currNode = map.map.table[i];
            while (currNode != nullptr) {
                Nodes* nextNode = currNode->next;
                writenode(currNode->value->value,currNode->value->key);
                delete currNode->value->value;
                delete currNode->value;
                delete currNode;
                currNode = nextNode;
            }
            map.map.table[i] = nullptr;
        }
        delete map.head;
        delete map.tail;
        opfile.close();
    }
    sjtu::vector<T> find(const keys &key){
        int num = finds(key,head.pos_of_root);
        node* temp;
        getnode(temp,num);
        int l=0,r=temp->now_num-1;
        while(l<r){
            int mid=(l+r)>>1;
            if(key<=temp->value[mid].key)r=mid;
            else l=mid+1;
        }
        int x=l;
        sjtu::vector<T> ans;
        if(temp->now_num==0)return ans;
        while(true){
            if(x==temp->now_num){
                if(temp->front_pos==0)break;
                getnode(temp,temp->front_pos);
                x=0;
            }
            //std::cout<<x<<'*'<<std::endl;
            if(temp->value[x].key==key)ans.push_back(temp->value[x].value);
            else if(key<temp->value[x].key)break;
            x++;
        }
        return ans;
    }
    void insert(const keys &index,const T &val){
        data obj(index,val);
        int pos=find_pos(obj,head.pos_of_root);
        node* temp;
        getnode(temp,pos);
        int t=sjtu::upper_bound(temp->value,temp->value+temp->now_num,obj)-temp->value;
        for(int i=temp->now_num;i>t;i--)temp->value[i]=temp->value[i-1];
        temp->value[t]=obj;
        temp->now_num++;
        if(temp->now_num==size_of_block)devideleaf(pos);
    }
    void erase(keys key,T val){
        data obj(key,val);
        int pos=find_pos(obj,head.pos_of_root);
        node* temp;
        getnode(temp,pos);
        int t=sjtu::upper_bound(temp->value,temp->value+temp->now_num,obj)-temp->value;
        if(!(temp->value[t-1]==obj))return;
        t--;
        if(t==0 && head.pos_of_root!=pos){
            if(temp->now_num!=1){
                node* fa;
                getnode(fa,temp->pos_of_fa);
                int t1=sjtu::upper_bound(fa->value,fa->value+fa->now_num,temp->value[0])-fa->value;
                if(t1!=0){
                    fa->value[t1-1]=temp->value[1];
                }
                else freshleft(fa->pos_of_fa,temp->value[1]);
            }
            else{
                node* fa;
                getnode(fa,temp->pos_of_fa);
                node *left,*right;
                if(!judge(temp->back_pos,temp->pos_of_fa,left)){
                    getnode(right,temp->front_pos);
                    int t1=sjtu::upper_bound(fa->value,fa->value+fa->now_num,temp->value[0])-fa->value;
                    if(t1!=0){
                        fa->value[t1-1]=right->value[0];
                    }
                    else freshleft(fa->pos_of_fa,right->value[0]);
                }
                else if(!judge(temp->front_pos,temp->pos_of_fa,right)){
                    int t1=sjtu::upper_bound(fa->value,fa->value+fa->now_num,temp->value[0])-fa->value;
                    if(t1!=0){
                        fa->value[t1-1]=left->value[left->now_num-1];
                    }
                    else freshleft(fa->pos_of_fa,left->value[left->now_num-1]);
                }
                else{
                    getnode(right,temp->front_pos);
                    int t1=sjtu::upper_bound(fa->value,fa->value+fa->now_num,temp->value[0])-fa->value;
                    if(t1!=0){
                        fa->value[t1-1]=right->value[0];
                    }
                    else freshleft(fa->pos_of_fa,right->value[0]);
                }
            }
        }
        for(int i=t;i<temp->now_num-1;i++)temp->value[i]=temp->value[i+1];
        temp->now_num--;
        if(temp->now_num<size_of_block/2-1)balanceleaf(pos);
    }
    bool empty()
    {
        node* root;
        getnode(root,head.pos_of_root);
        if(root->type==leaf && root->now_num==0)return true;
        else return false;
    }
    // void prints(){
    //     node root;
    //     getnode(root,head.pos_of_root);
    //     std::cout<<std::endl<<"***********"<<std::endl;
    //     for(int i=0; i< root.now_num;i++)std::cout<<root.value[i].key<<std::endl;
    //     std::cout<< "***********"<<std::endl;
    // }
};

#endif