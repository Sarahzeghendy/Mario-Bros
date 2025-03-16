#pragma once
#include <SFML/Graphics.hpp>

class FireFlower
{
public:
    FireFlower(float x, float y);

    void draw(sf::RenderWindow &window);
    sf::FloatRect getBounds() const;
    bool isCollected() const;
    void collect();

private:
    sf::Sprite sprite;
    sf::Texture texture;
    bool collected;
};
