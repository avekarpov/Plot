#include "PlotManager.hpp"

#include <SFML/OpenGL.hpp>

PlotManager::PlotManager(std::string windowTitle, sf::Vector2u windowSize, const std::string& fontPath) : _windowTitle(std::move(windowTitle)), _windowSize(windowSize)
{
    _font.loadFromFile(fontPath);
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
    PlotInformation plotInformation;
    
    sf::RenderWindow window(sf::VideoMode(_windowSize.x, _windowSize.y), _windowTitle.c_str(), sf::Style::Close | sf::Style::Titlebar);
    sf::Clock frameTime;
    
    sf::Vector2i windowMousePosition;
    sf::Vector2i previousWindowMousePosition = sf::Mouse::getPosition(window);
    
    glViewport(0, 0, (int)_windowSize.x, (int)_windowSize.y);
    
    plotInformation.left = left;
    plotInformation.right = right;
    plotInformation.bottom = bottom;
    plotInformation.top = top;
    
    while(window.isOpen())
    {
        _handleWindowEvents(plotInformation, window);
        
        windowMousePosition = sf::Mouse::getPosition(window);
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            _move(plotInformation, windowMousePosition, previousWindowMousePosition);
        }
        previousWindowMousePosition = windowMousePosition;
        
        if(frameTime.getElapsedTime() > sf::milliseconds((signed int)(1000.0 / FRAME_RATE)))
        {
            frameTime.restart();
    
            plotInformation.mousePosition = _plotMousePosition(plotInformation, windowMousePosition);
            
            window.clear();
    
            _glDraw(plotInformation);
            _sfmlDraw(plotInformation, window);
            
            window.display();
        }
    }
}

void PlotManager::setWindowTitle(std::string windowTitle) noexcept
{
    _windowTitle = std::move(windowTitle);
}

void PlotManager::setWindowSize(sf::Vector2u windowSize) noexcept
{
    _windowSize = windowSize;
}

const std::string &PlotManager::getWindowTitle() const noexcept
{
    return _windowTitle;
}

sf::Vector2u PlotManager::getWindowSize() const noexcept
{
    return _windowSize;
}

void PlotManager::_move(PlotInformation &plotInformation, sf::Vector2i windowMousePosition, sf::Vector2i previousWindowMousePosition) const
{
    static sf::Vector2f positionDifferent;
    
    positionDifferent.x = (float)(windowMousePosition.x - previousWindowMousePosition.x) * ((plotInformation.right - plotInformation.left) / (float)_windowSize.x);
    positionDifferent.y = (float)(windowMousePosition.y - previousWindowMousePosition.y) * ((plotInformation.top - plotInformation.bottom) / (float)_windowSize.y);
    
    plotInformation.left   -= positionDifferent.x;
    plotInformation.right  -= positionDifferent.x;
    plotInformation.bottom += positionDifferent.y;
    plotInformation.top    += positionDifferent.y;
}

void PlotManager::_handleWindowEvents(PlotManager::PlotInformation &plotInformation, sf::RenderWindow &window) const
{
    static sf::Event event;
    
    while(window.pollEvent(event))
    {
        switch(event.type)
        {
            case sf::Event::Closed:
            {
                window.close();
                
                break;
            }
            
            case sf::Event::MouseWheelScrolled:
            {
                if(event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
                {
                    float delta = event.mouseWheelScroll.delta;
                    
                    if(plotInformation.right - plotInformation.left > 2.0 * delta && plotInformation.top - plotInformation.bottom > 2.0 * delta)
                    {
                        plotInformation.left   += delta;
                        plotInformation.right  -= delta;
                        plotInformation.bottom += delta * ((float)_windowSize.y / (float)_windowSize.x);
                        plotInformation.top    -= delta * ((float)_windowSize.y / (float)_windowSize.x);
                    }
                }
                
                break;
            }
        }
    }
}

sf::Vector2f PlotManager::_plotMousePosition(const PlotInformation &plotInformation, sf::Vector2i windowMousePosition) const
{
    sf::Vector2f plotMousePosition;
    
    plotMousePosition.x = (float)windowMousePosition.x;
    plotMousePosition.y = (float)_windowSize.y - (float)windowMousePosition.y;
    plotMousePosition.x /= (float)_windowSize.x / (plotInformation.right - plotInformation.left);
    plotMousePosition.y /= (float)_windowSize.y / (plotInformation.top - plotInformation.bottom);
    plotMousePosition.x += plotInformation.left;
    plotMousePosition.y += plotInformation.bottom;
    
    return plotMousePosition;
}

void PlotManager::_glDraw(const PlotInformation &plotInformation) noexcept
{
    glLoadIdentity();
    glOrtho(plotInformation.left, plotInformation.right, plotInformation.bottom, plotInformation.top, 1, -1);
    
    glBegin(GL_LINES);
    {
        glColor3f(1, 0, 0);
        glVertex2f(plotInformation.left, 0);
        glVertex2f(plotInformation.right, 0);
        
        glColor3f(0, 1, 0);
        glVertex2f(0, plotInformation.bottom);
        glVertex2f(0, plotInformation.top);
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
}

sf::Text PlotManager::_textInstance()
{
    sf::Text text;
    
    text.setFont(_font);
    text.setFillColor(sf::Color::Red);
    text.setCharacterSize(TEXT_SIZE);
    
    return text;
}

void PlotManager::_sfmlDraw(const PlotManager::PlotInformation &plotInformation, sf::RenderWindow &window) noexcept
{
    static auto mousePositionText = _textInstance();
    static auto plotInformationText = _textInstance();
    
    plotInformationText.setString("left: " + std::to_string(plotInformation.left) + " right: " + std::to_string(plotInformation.right) + '\n' + "bottom: " + std::to_string(plotInformation.bottom) + " top: " + std::to_string(plotInformation.top));
    plotInformationText.setPosition({0, 0});
    
    mousePositionText.setString("x: " + std::to_string(plotInformation.mousePosition.x) + " y: " + std::to_string(plotInformation.mousePosition.y));
    mousePositionText.setPosition({0, 40});
    
    window.pushGLStates();
    
    window.draw(plotInformationText);
    window.draw(mousePositionText);
    
    window.popGLStates();
}

PlotManager::PlotManagerException::PlotManagerException(std::string message) : _message(std::move(message))
{

}

const char *PlotManager::PlotManagerException::what() const
{
    return _message.c_str();
}
