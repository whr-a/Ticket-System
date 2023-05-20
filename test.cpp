#include "utility.hpp"
#include <string>
#include <cstring>
#include "vector.hpp"
#include "Tokenscanner.hpp"
#include "database_copy.hpp"
struct node
{
    char x[21];
    int p;
    node(){}
    node(const node &obj):p(obj.p){
        strcpy(x,obj.x);
    }
    node(const char* x_,int p_):p(p_){
        strcpy(x,x_);
    }
};
int main ()
{
    database<int,int> db("11111111111.db");
    db.insert(1,1);
    db.insert(2,2);
    db.insert(3,3);
    std::cout<<db.find(3)[0];
}