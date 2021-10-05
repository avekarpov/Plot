#ifndef PLOT_HPP
#define PLOT_HPP

#include <SFML/Graphics.hpp>

#include <string>

#include <functional>
#include <vector>

#include <exception>

class FunctionManager
{
public:
    using Point2f = sf::Vector2f;
    
    FunctionManager(std::function<float(float)> function, float from, float to, float h);
    
    void setFunction(std::function<float(float)> function) noexcept;
    void setFrom(float from) noexcept;
    void setTo(float to) noexcept;
    void setH(float h);
    void setPoints(std::vector<Point2f> points) noexcept;
    
    const std::function<float(float)> &getFunction() const noexcept;
    float getFrom() const noexcept;
    float getTo() const noexcept;
    float getH() const noexcept;
    const std::vector<Point2f> &getPoints() const noexcept;
    
    const std::vector<Point2f> &calculate() noexcept;
    
    class FunctionsManagerException : public std::exception
    {
    public:
        explicit FunctionsManagerException(std::string message);
        
        const char * what() const override;
        
    private:
        std::string _message;
    };
    
private:
    float _from, _to;
    float _h;
    
    std::function<float(float)> _function;
    
    std::vector<Point2f> _points;
    
};


#endif //PLOT_HPP
