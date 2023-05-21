#include "utility.hpp"
#include <string>
#include <cstring>
#include "vector.hpp"
#include "Tokenscanner.hpp"
#include "unordered_map.hpp"
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
    MyUnorderedMap<int,int,128> mp;
    mp.insert(1,1);
    mp.insert(2,2);
    std::cout<<mp[2];
}