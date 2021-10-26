#include "FunctionManager.hpp"
#include "PlotManager.hpp"

#include <thread>

#include <iostream>

#include <map>

int main(int argc, char *argv[])
{
    static std::map<std::string, std::function<float(float)>> functions;
    
    functions.insert({"sin", [](float x){
        return std::sin(x);
    }});
    functions.insert({"cos", [](float x){
        return std::cos(x);
    }});
    functions.insert({"tan", [](float x){
        return std::tan(x);
    }});
    
    FunctionManager functionManager([](float x){
        return std::sin(x);
    }, -100, 100, 1e-3);
    PlotManager plotManager("sin", {1200, 800}, "C:/Windows/Fonts/arial.ttf");
    
    bool isStop = false;
    
    std::thread displayThread([&](){
        plotManager.display(-100, 100, isStop);
    });
    displayThread.detach();
    
    plotManager.setPoints(functionManager.calculate());
    
    std::string command;
    
    do
    {
        std::cin >> command;
        
        if(command == "cf")
        {
            std::string functionName;
            
            std::cin >> functionName;
            
            auto function = functions.find(functionName);
            
            if(function != functions.end())
            {
                plotManager.setWindowTitle(function->first);
                functionManager.setFunction(function->second);
                std::cout << "Start calculating" << std::endl;
                plotManager.setPoints(functionManager.calculate());
                std::cout << "End calculating" << std::endl;
            }
            else
            {
                std::cout << "Invalid function\nChose one of this:" << std::endl;
                for(const auto &item: functions)
                {
                    std::cout << item.first << std::endl;
                }
            }
            
            continue;
        }
        else if(command == "sft")
        {
            float from, to;
            
            std::cin >> from >> to;
            
            if(to < from)
            {
                std::cout << "Invalid from to (to < from)" << std::endl;
                
                continue;
            }
                functionManager.setFrom(from);
                functionManager.setTo(to);
                
                std::cout << "Start calculating" << std::endl;
                plotManager.setPoints(std::move(functionManager.calculate()));
                std::cout << "End calculating" << std::endl;
        }
        else if(command == "cc")
        {
            unsigned short r, b, g;
            
            std::cin >> r >> b >> g;
            
            if(255 < r || 255 < b || 255 < g)
            {
                std::cout << "Invalid color (one of color param more 255)" << std::endl;
                
                continue;
            }
            
            plotManager.setWindowColor({
                static_cast<unsigned char>(r),
                static_cast<unsigned char>(b),
                static_cast<unsigned char>(g)});
        }
        else if(command == "h")
        {
            std::cout << "q                  - exit\n"
                         "cf [function name] - change function\n"
                         "sft [from] [to]    - set from to\n"
                         "cc [r] [b] [g]     - change window colour\n"
                         "h                  - this message\n";
        }
        else if(command == "q")
        {
            break;
        }
        else
        {
            std::cout << "Wrong command, input \"h\" for help" << std::endl;
        }
    }
    while(true);
    
    isStop = true;
    
    return 0;
}