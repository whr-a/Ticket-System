#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <cmath>
#include <fstream>
#include "database.hpp"
#include "vector.hpp"
#include "Tokenscanner.hpp"
void processLine(std::string &line);
int main ()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    while (true) {
        std::string input;
        if(!getline(std::cin, input))return 0;
        processLine(input);
    }
}
void processLine(std::string &line) {
    Tokenscanner scanner;
    scanner.setInput(line);
    std::string s=scanner.nextToken();
    if(s.empty())return;

}