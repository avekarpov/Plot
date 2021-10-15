#include "FunctionManager.hpp"
#include "PlotManager.hpp"

#include <thread>

#include <iostream>

#include <map>

int main(int argc, char *argv[])
{
    static std::map<std::string, std::function<float(float)>> functions;
    
    functions.insert({"sin", [](float x){return std::sin(x);}});
    functions.insert({"cos", [](float x){return std::cos(x);}});
    functions.insert({"tan", [](float x){return std::tan(x);}});
    
    float from = -100, to = 100;
    float h = 1e-3;
    
    float left = -100, right = 100;
    
    FunctionManager functionManager([](float x){return std::sin(x);}, from, to, h);
    PlotManager plotManager("Plot", {1200, 800}, "C:/Windows/Fonts/arial.ttf");
    
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
        
        if(command == "cf")
        {
            static std::string functionName;
            
            std::cin >> functionName;
            
            auto function = functions.find(functionName);
            
            if(function != functions.end())
            {
                plotManager.setWindowTitle(function->first);
                functionManager.setFunction(function->second);
                plotManager.setPoints(functionManager.calculate());
            }
        }
    }
    while(command != "q");
    
    isStop = true;
    
    return 0;
}