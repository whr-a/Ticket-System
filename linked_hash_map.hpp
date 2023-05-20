#ifndef LINKED_HASH_MAP
#define LINKED_HASH_MAP
#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <cmath>

template <typename Key, typename Value>
class LinkedHashMap {
public:
    //LinkedHashMap节点定义
    struct Node {
        Key key;
        Value value;
        Node* prev;
        Node* next;

        Node()
            : prev(nullptr), next(nullptr) {}

        Node(const Key& k, const Value& v)
            : key(k), value(v), prev(nullptr), next(nullptr) {}
    };
    // 哈希表节点定义
    struct Nodes {
        Key key;
        Node* value;
        Nodes* next;

        Nodes(const Key& k,Node* v)
            : key(k), value(v), next(nullptr) {}
    };
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
        Node*& operator[](const Key& key) {
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
    // 构造函数
    LinkedHashMap() {
        head = new Node();
        tail = new Node();
        head->next = tail;
        tail->prev = head;
    }

    // 析构函数
    ~LinkedHashMap() {
        clear();
        delete head;
        delete tail;
    }

    // 插入键值对
    void insert(const Key& key, const Value& value) {
        if (map.contains(key)) {
            // 键已存在，更新值并将节点移到链表头部
            Node* node = map[key];
            node->value = value;
            moveToHead(node);
        } else {
            // 键不存在，创建新节点并插入到链表头部
            Node* node = new Node(key, value);
            map[key] = node;
            insertToHead(node);

            // 检查容量是否超出限制
            if (map.size() > capacity) {
                // 超出限制，移除链表尾部的节点
                Node* tailNode = removeTail();
                map.erase(tailNode->key);
                delete tailNode;
            }
        }
    }

    // 删除键值对
    void erase(const Key& key) {
        if (map.contains(key)) {
            Node* node = map[key];
            removeNode(node);
            map.erase(key);
            delete node;
        }
    }

    // 获取值
    Value*& operator[](const Key& key) {//危险，要先contains一下才能用！！！
        Node* node = map[key];
        moveToHead(node);
        return &node->value;
    }

    // 判断键是否存在
    bool contains(const Key& key) const {
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
    
private:
    Node* head;  // 链表头节点
    Node* tail;  // 链表尾节点
    MyUnorderedMap map;  // 哈希表，用于快速查找节点
    static const size_t capacity = 2;  // 容量限制，可以根据需求进行调整

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

#endif

// template <typename Key, typename Value, int TableSize>
// class MyUnorderedMap {
// public:
//     // 构造函数
//     MyUnorderedMap() {
//         table = new Nodes*[TableSize]();
//     }

//     // 析构函数
//     ~MyUnorderedMap() {
//         clear();
//         delete[] table;
//     }

//     // 插入键值对
//     void insert(const Key& key, const Value& value) {
//         int index = hashFunction(key);
//         Nodes* newNode = new Nodes(key, value);
//         newNode->next = table[index];
//         table[index] = newNode;
//     }

//     // 删除键值对
//     void erase(const Key& key) {
//         int index = hashFunction(key);
//         Nodes* currNode = table[index];
//         Nodes* prevNode = nullptr;

//         while (currNode != nullptr) {
//             if (currNode->key == key) {
//                 if (prevNode != nullptr)
//                     prevNode->next = currNode->next;
//                 else
//                     table[index] = currNode->next;

//                 delete currNode;
//                 break;
//             }

//             prevNode = currNode;
//             currNode = currNode->next;
//         }
//     }

//     // 获取值
//     Value& operator[](const Key& key) {
//         int index = hashFunction(key);
//         Nodes* currNode = table[index];

//         while (currNode != nullptr) {
//             if (currNode->key == key)
//                 return currNode->value;

//             currNode = currNode->next;
//         }

//         Nodes* newNode = new Nodes(key, Value());
//         newNode->next = table[index];
//         table[index] = newNode;

//         return newNode->value;
//     }

//     // 判断键是否存在
//     bool contains(const Key& key) const {
//         int index = hashFunction(key);
//         Nodes* currNode = table[index];

//         while (currNode != nullptr) {
//             if (currNode->key == key)
//                 return true;

//             currNode = currNode->next;
//         }

//         return false;
//     }

//     // 获取键值对数量
//     size_t size() const {
//         size_t count = 0;

//         for (int i = 0; i < TableSize; i++) {
//             Nodes* currNode = table[i];
//             while (currNode != nullptr) {
//                 count++;
//                 currNode = currNode->next;
//             }
//         }

//         return count;
//     }

//     // 清空哈希表
//     void clear() {
//         for (int i = 0; i < TableSize; i++) {
//             Nodes* currNode = table[i];
//             while (currNode != nullptr) {
//                 Nodes* nextNode = currNode->next;
//                 delete currNode;
//                 currNode = nextNode;
//             }
//             table[i] = nullptr;
//         }
//     }


//     // 哈希表节点定义
//     struct Nodes {
//         Key key;
//         Value value;
//         Nodes* next;

//         Nodes(const Key& k, const Value& v)
//             : key(k), value(v), next(nullptr) {}
//     };

//     Nodes** table;  // 哈希表数组

//     // 哈希函数
//     int hashFunction(const Key& key) const {
//         return std::hash<Key>()(key) % TableSize;
//     }
// };
