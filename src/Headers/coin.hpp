#ifndef COIN_HPP
#define COIN_HPP

#include <SFML/Graphics.hpp>


class Coin {
private:

    sf::Texture texture;
    sf::Sprite sprite;
    bool collected; 

public:

    Coin(float x, float y);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const; 
    bool isCollected() const;
    void collect();
};

#endif
