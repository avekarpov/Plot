#include <iostream>
#include <cmath>

#include "Plot.hpp"

int main(int argc, char *argv[])
{
    float from = 0, to = 1, h = 1e-6;
    
    FunctionManager functionManager([](float x){return std::sin(x);}, from, to, h);
    
    functionManager.calculate();
    
    return 0;
}