#include "FunctionManager.hpp"

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
