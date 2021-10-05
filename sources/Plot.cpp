#include "Plot.hpp"

#include <utility>

#include <SFML/OpenGL.hpp>
#include <iostream>

FunctionManager::FunctionManager(std::function<float(float)> function, float from, float to, float h) : _function(std::move(function)), _from(from), _to(to), _h(h)
{

}

void FunctionManager::setFunction(std::function<float(float)> function) noexcept
{
    _function = std::move(function);
}

void FunctionManager::setFrom(float from) noexcept
{
    _from = from;
}

void FunctionManager::setTo(float to) noexcept
{
    _to = to;
}

void FunctionManager::setH(float h)
{
    if(h <=0 )
    {
        throw FunctionsManagerException("\'h\' is less than or equal to zero");
    }
    
    _h = h;
}

const std::function<float(float)> &FunctionManager::getFunction() const noexcept
{
    return _function;
}

float FunctionManager::getFrom() const noexcept
{
    return _from;
}

float FunctionManager::getTo() const noexcept
{
    return _to;
}

float FunctionManager::getH() const noexcept
{
    return _h;
}

std::vector<Point2f> FunctionManager::calculate() const
{
    if(_from > _to)
    {
        throw FunctionsManagerException("Incorrect segment: from > to");
    }
    
    float x = _from;
    
    std::vector<Point2f> points;
    
    points.reserve((unsigned)ceil((_to - _from) / _h) + 1);
    
    while(std::abs(x - (_to + 0.5 * _h)) > _h)
    {
        points.emplace_back(x, _function(x));
        
        x += _h;
    }
    
    return points;
}

float FunctionManager::_basicFunction(float x) noexcept
{
    return x;
}

FunctionManager::FunctionsManagerException::FunctionsManagerException(std::string message) : _message(std::move(message))
{

}

const char *FunctionManager::FunctionsManagerException::what() const
{
    return _message.c_str();
}

PlotManager::PlotManager(std::string windowTitle, sf::Vector2u windowSize) : _windowTitle(std::move(windowTitle)), _windowSize(windowSize)
{

}

void PlotManager::setPoints(std::vector<Point2f> points) noexcept
{
    _points = std::move(points);
}

const std::vector<Point2f> &PlotManager::getPoints() const noexcept
{
    return _points;
}

void PlotManager::display(float left, float right, float bottom, float top)
{
    sf::RenderWindow window(sf::VideoMode(_windowSize.x, _windowSize.y), _windowTitle.c_str(), sf::Style::Close | sf::Style::Titlebar);
    sf::Event event;
    
    sf::Clock frameTime;

    window.setFramerateLimit(FRAME_RATE);
    
    glViewport(0, 0, (int)_windowSize.x, (int)_windowSize.y);
    
    while(window.isOpen())
    {
        while(window.pollEvent(event))
        {
            switch(event.type)
            {
                case sf::Event::Closed:
                {
                    window.close();
                    
                    break;
                }
            }
        }
        
        if(frameTime.getElapsedTime() > sf::milliseconds((signed int)(1000.0 / FRAME_RATE)))
        {
            frameTime.restart();
    
            glLoadIdentity();
            glOrtho(left, right, bottom, top, 1, -1);
    
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            glBegin(GL_LINES);
            {
                glColor3f(1, 0, 0);
                glVertex2f(left, 0);
                glVertex2f(right, 0);
        
                glColor3f(0, 1, 0);
                glVertex2f(0, bottom);
                glVertex2f(0, top);
            }
            glEnd();
    
    
            glBegin(GL_LINE_STRIP);
            {
                glColor3f(1, 1, 1);
                for(const auto &point: _points)
                {
                    glVertex2f(point.x, point.y);
                }
            }
            glEnd();
            
            window.display();
        }
    }
}

PlotManager::PlotManagerException::PlotManagerException(std::string message) : _message(std::move(message))
{

}

const char *PlotManager::PlotManagerException::what() const
{
    return _message.c_str();
}
