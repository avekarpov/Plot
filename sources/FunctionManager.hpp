#ifndef FUNCTIONMANAGER_HPP
#define FUNCTIONMANAGER_HPP

#include <SFML/Graphics.hpp>

#include <functional>

using Point2f = sf::Vector2f;

class FunctionManager
{
public:
    explicit FunctionManager(std::function<float(float)> function = _basicFunction, float from = BASIC_FROM , float to = BASIC_TO, float h = BASIC_H);
    
    void setFunction(std::function<float(float)> function) noexcept;
    void setFrom(float from) noexcept;
    void setTo(float to) noexcept;
    void setH(float h);
    
    const std::function<float(float)> &getFunction() const noexcept;
    float getFrom() const noexcept;
    float getTo() const noexcept;
    float getH() const noexcept;
    
    std::vector<Point2f> calculate() const;
    
    class FunctionsManagerException : public std::exception
    {
    public:
        explicit FunctionsManagerException(std::string message);
        
        const char *what() const override;
    
    private:
        std::string _message;
        
    };

private:
    static constexpr float BASIC_FROM = 0.0;
    static constexpr float BASIC_TO = 1.0;
    static constexpr float BASIC_H = 1e-6;
    
    float _from, _to;
    float _h;
    
    std::function<float(float)> _function;
    
    static float _basicFunction(float x) noexcept; // y = x
    
};

#endif //FUNCTIONMANAGER_HPP
