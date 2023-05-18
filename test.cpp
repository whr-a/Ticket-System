#include "utility.hpp"
#include <string>
#include <cstring>
#include "vector.hpp"
#include "database.hpp"
#include "Tokenscanner.hpp"
int main ()
{
    Tokenscanner scanner;
    std::cout<<scanner.check_num("123")+scanner.check_num("456");
}