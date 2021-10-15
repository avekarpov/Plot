#ifndef PLOTMANAGER_HPP
#define PLOTMANAGER_HPP

#include <SFML/Graphics.hpp>

using Point2f = sf::Vector2f;

class PlotManager
{
public:
    PlotManager(std::string windowTitle, sf::Vector2u windowSize, const std::string& fontPath);
    
    void setPoints(std::vector<Point2f> points) noexcept;
    void setWindowTitle(std::string windowTitle) noexcept;
    void setWindowSize(sf::Vector2u windowSize) noexcept;
    
    const std::vector<Point2f> &getPoints() const noexcept;
    const std::string &getWindowTitle() const noexcept;
    sf::Vector2u getWindowSize() const noexcept;
    
    void display(float left, float right, const bool &isStop = false);
    
    class PlotManagerException : public std::exception
    {
    public:
        explicit PlotManagerException(std::string message);
        
        const char *what() const override;
        
    private:
        std::string _message;
    
    };
    
private:
    struct PlotInformation
    {
        float left, right, bottom, top;
        sf::Vector2f mousePosition;
    };
    
    std::vector<Point2f> _points;
    
    std::string _windowTitle;
    sf::Vector2u _windowSize;
    
    sf::Font _font;
    
    static const unsigned FRAME_RATE = 60;
    static const unsigned TEXT_SIZE = 12;
    
    void _move(PlotInformation &plotInformation, sf::Vector2i windowMousePosition, sf::Vector2i previousWindowMousePosition) const;
    
    void _handleWindowEvents(PlotInformation &plotInformation, sf::RenderWindow &window) const;
    
    sf::Vector2f _plotMousePosition(const PlotInformation &plotInformation, sf::Vector2i windowMousePosition) const;
    
    void _glDraw(const PlotInformation &plotInformation) noexcept;
    void _sfmlDraw(const PlotInformation &plotInformation, sf::RenderWindow &window) noexcept;
    
    sf::Text _textInstance();
    
};


#endif //PLOTMANAGER_HPP
