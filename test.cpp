#include "utility.hpp"
#include <string>
#include <cstring>
#include "vector.hpp"
int main ()
{
    sjtu::vector<int> t;
    t.push_back(33);
    t.push_back(42);
    t.push_back(82);
    t.push_back(37);
    t.push_back(51);
    t.push_back(61);
    t.sort();
    for(int i=0;i<t.size();i++)std::cout<<t[i]<<'\n'; 
}