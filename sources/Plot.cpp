#include "Plot.hpp"

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

void FunctionManager::setPoints(std::vector<Point2f> points) noexcept
{
    _points = std::move(points);
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

const std::vector<FunctionManager::Point2f> &FunctionManager::calculate()
{
    if(_from > _to)
    {
        throw FunctionsManagerException("Incorrect segment: from > to");
    }
    
    float x = _from;
    
    _points.clear();
    _points.reserve((unsigned)ceil((_to - _from) / _h) + 1);
    
    while(std::abs(x - (_to + 0.5 * _h)) > std::numeric_limits<float>::epsilon())
    {
        _points.emplace_back(x, _function(x));
        
        x += _h;
    }
    
    return _points;
}

const std::vector<FunctionManager::Point2f> &FunctionManager::getPoints() const noexcept
{
    return _points;
}

float FunctionManager::_basicFunction(float x) noexcept
{
    return x;
}

FunctionManager::FunctionsManagerException::FunctionsManagerException(std::string message) : _message(message)
{

}

const char *FunctionManager::FunctionsManagerException::what() const
{
    return _message.c_str();
}
