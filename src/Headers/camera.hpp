#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <SFML/Graphics.hpp>

class Camera 
{
    public:
        Camera(float width, float height);
        void setCenter(float x, float y);  
        void update(const sf::Vector2u& windowSize);
        const sf::View& getView() const;
        void clampPosition(float& x, float& y);

    private:
        sf::View view;
        float defaultWidth;
        float defaultHeight;
        float minX;
        float maxX;
        float minY;
        float maxY;
};

#endif 
