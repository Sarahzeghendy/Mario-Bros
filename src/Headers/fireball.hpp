#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Fireball
{
public:
    Fireball(float x, float y, float direction);

    void update(const std::vector<sf::Sprite> &blocks);
    void draw(sf::RenderWindow &window);
    sf::FloatRect getBounds() const;
    bool isActive() const;
    void destroy();

private:
    sf::Sprite sprite;
    sf::Texture texture;
    float speed;
    float gravity;
    float velocityY;
    bool active;
    float initialX;  
};
