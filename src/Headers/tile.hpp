#ifndef TILE_H
#define TILE_H

#include <SFML/Graphics.hpp>

class Tile {
private:
    sf::Sprite sprite;
    bool solid; // Can Mario collide with it?

public:
    Tile(const sf::Texture &texture, bool isSolid, float x, float y) {
        sprite.setTexture(texture);
        sprite.setPosition(x, y);
        solid = isSolid;
    }

    void draw(sf::RenderWindow &window) {
        window.draw(sprite);
    }

    bool isSolid() const { return solid; }
    sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }
};

#endif
