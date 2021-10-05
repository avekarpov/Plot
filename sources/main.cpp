#include <iostream>
#include <cmath>

#include "Plot.hpp"

int main(int argc, char *argv[])
{
    float left = -4, right = 2, bottom = -2, top = 2;
    float h = 1e-2;
    
    auto sin = [](float x)->float{return std::sin(x);};
    
    FunctionManager functionManager(sin, left, right, h);
    PlotManager plotManager("Plot", {800, 800});
    
    plotManager.setPoints(functionManager.calculate());
    plotManager.display(left, right, bottom, top);
    
    return 0;
}