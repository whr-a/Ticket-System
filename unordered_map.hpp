#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <cmath>
template <typename Key, typename Value,size_t TableSize>
class MyUnorderedMap {
public:
    struct Nodes {
        Key key;
        Value value;
        Nodes* next;
        Nodes(const Key& k,const Value &v)
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
    void insert(const Key& key, const Value& value) {
        int index = hashFunction(key);
        Nodes* newNode = new Nodes(key, value);
        newNode->next = table[index];
        table[index] = newNode;
    }

    // 删除键值对
    void erase(const Key& key) {
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
    Value& operator[](const Key& key) {
        int index = hashFunction(key);
        Nodes* currNode = table[index];

        while (currNode != nullptr) {
            if (currNode->key == key)
                return currNode->value;
            currNode = currNode->next;
        }

        Nodes* newNode = new Nodes(key,Value());
        newNode->next = table[index];
        table[index] = newNode;

        return newNode->value;
    }

    // 判断键是否存在
    bool contains(const Key& key) const {
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
    int hashFunction(const Key& key) const {
        return std::hash<Key>()(key) % TableSize;
    }
};