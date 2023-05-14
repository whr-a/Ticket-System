#include "utility.hpp"
#include <string>
#include <cstring>
#include "vector.hpp"
int main ()
{
    Time x(8,1,0,0);
    Time y(6,30,23,59);
    std::cout<<x-y;
}