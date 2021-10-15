#include "FunctionManager.hpp"
#include "PlotManager.hpp"

#include <thread>

#include <iostream>

int main(int argc, char *argv[])
{
    float from = -100, to = 100;
    float h = 1e-3;
    
    float left = -100, right = 100;
    
    FunctionManager functionManager([](float x){return std::sin(x);}, from, to, h);
    PlotManager plotManager("Plot", {800, 400}, "C:/Windows/Fonts/arial.ttf");
    
    bool isStop = false;

    std::thread displayThread([&]()
    {
        plotManager.display(left, right, isStop);
    });
    displayThread.detach();
    
    plotManager.setPoints(functionManager.calculate());
    
    std::string command;
    
    do
    {
        std::cin >> command;
    }
    while(command != "q");
    
    isStop = true;
    
    return 0;
}