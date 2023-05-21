#ifndef TRAIN_DATABASE
#define TRAIN_DATABASE
#include <fstream>
#include <iostream>
#include <cstring>
#include <string>
#include <cmath>
#include <cstdio>
#include "vector.hpp"

struct Node {
    int data;
    Node* next;
};

// 队列类
class Queue {
public:
    Node* front; // 队首指针
    Node* rear;  // 队尾指针
    Queue() : front(nullptr), rear(nullptr) {}
    ~Queue() {
        while (front != nullptr) {
            Node* temp = front;
            front = front->next;
            delete temp;
        }
    }
    // 判断队列是否为空
    bool isEmpty() {
        return (front == nullptr);
    }

    // 入队
    void enqueue(int value) {
        Node* newNode = new Node();
        newNode->data = value;
        newNode->next = nullptr;

        // 如果队列为空，新节点既是队首也是队尾
        if (isEmpty()) {
            front = rear = newNode;
        } else {
            // 将新节点添加到队尾，并更新队尾指针
            rear->next = newNode;
            rear = newNode;
        }
    }

    // 出队
    void dequeue() {
        if (isEmpty()) return;
        Node* temp = front; // 保存当前队首节点的指针
        front = front->next; // 更新队首指针

        // 如果队列只有一个节点，出队后队列为空，需要更新队尾指针
        if (front == nullptr) {
            rear = nullptr;
        }

        delete temp; // 释放原队首节点的内存
    }

    // 获取队首元素
    int getFront() {
        if (isEmpty()) return -1;
        return front->data;
    }

};

class vec_block
{
public:
    int data[20000];
    vec_block(){memset(data,0,sizeof(data));}
    vec_block(const vec_block &obj){
        for(int i=0;i<20000;i++)data[i]=obj.data[i];
    }
    vec_block operator= (const vec_block &obj){
        for(int i=0;i<20000;i++)data[i]=obj.data[i];
        return *this;
    }
};
template <class T>
class train_database
{
public:
    std::fstream opfile;
    int max_num;
    Queue q;
    train_database(){
        std::ifstream in;
        in.open("train_data.db");
        if(!in){
            std::ofstream outfile("train_data.db");
            outfile.seekp(0);
            int t1=1;
            outfile.write(reinterpret_cast<char*>(&t1),sizeof(int));
            vec_block tem;
            outfile.seekp(sizeof(int));
            outfile.write(reinterpret_cast<char*>(&tem),sizeof(vec_block));
        }

        opfile.open("train_data.db");
        opfile.seekg(0);
        opfile.read(reinterpret_cast<char*>(&max_num),sizeof(int));
        opfile.seekg(sizeof(int));
        vec_block temp;
        opfile.read(reinterpret_cast<char*>(&temp),sizeof(vec_block));
        q.front=nullptr;
        q.rear=nullptr;
        for(int i=0;i<20000;i++){
            if(temp.data[i]==0)break;
            q.enqueue(temp.data[i]);
        }
    }
    ~train_database(){
        opfile.seekp(0);
        opfile.write(reinterpret_cast<char*>(&max_num),sizeof(int));
        vec_block temp;
        int cnt=-1;
        while(!q.isEmpty()){
            temp.data[++cnt]=q.getFront();
            q.dequeue();
        }
        opfile.seekp(sizeof(int));
        opfile.write(reinterpret_cast<char*>(&temp),sizeof(vec_block));
    }
    int insert(const T &obj){
        T obj_(obj);
        if(q.isEmpty()){
            opfile.seekp(sizeof(int)+sizeof(vec_block)+(max_num-1)*sizeof(T));
            opfile.write(reinterpret_cast<char*>(&obj_),sizeof(T));
            ++max_num;
            return max_num-1;
        }
        else{
            int x=q.getFront();
            q.dequeue();
            opfile.seekp(sizeof(int)+sizeof(vec_block)+(x-1)*sizeof(T));
            opfile.write(reinterpret_cast<char*>(&obj_),sizeof(T));
            return x;
        }
    }
    T find(int num){
        T temp;
        opfile.seekg(sizeof(int)+sizeof(vec_block)+(num-1)*sizeof(T));
        opfile.read(reinterpret_cast<char*>(&temp),sizeof(T));
        return temp;
    } 
    void erase(int num){
        q.enqueue(num);
    }
    void clear(){
        std::ofstream file("train_data.db", std::ios::trunc);
        file.close();
        while(!q.isEmpty())q.dequeue();
        std::ifstream in;
        in.open("train_data.db");
        if(!in){
            std::ofstream outfile("train_data.db");
            outfile.seekp(0);
            int t1=1;
            outfile.write(reinterpret_cast<char*>(&t1),sizeof(int));
            vec_block tem;
            outfile.seekp(sizeof(int));
            outfile.write(reinterpret_cast<char*>(&tem),sizeof(vec_block));
        }
        opfile.open("train_data.db");
        opfile.seekg(0);
        opfile.read(reinterpret_cast<char*>(&max_num),sizeof(int));
        opfile.seekg(sizeof(int));
        vec_block temp;
        opfile.read(reinterpret_cast<char*>(&temp),sizeof(vec_block));
        q.front=nullptr;
        q.rear=nullptr;
        for(int i=0;i<20000;i++){
            if(temp.data[i]==0)break;
            q.enqueue(temp.data[i]);
        }
    }
};

#endif