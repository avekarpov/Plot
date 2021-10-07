#include "FunctionManager.hpp"
#include "PlotManager.hpp"

int main(int argc, char *argv[])
{
    float left = -100, right = 100, bottom = -60, top = 60;
    float h = 1e-3;
    
    FunctionManager functionManager;
    PlotManager plotManager("Plot", {2000, 1200}, "C:/Windows/Fonts/arial.ttf");
    
    plotManager.setPoints(functionManager.calculate());
    plotManager.display(left, right, bottom, top);
    
    return 0;
}