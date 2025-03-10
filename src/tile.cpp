#include "Headers/tile.hpp"

// Constructor: Sets texture, position, and solidity
Tile::Tile(const sf::Texture &texture, bool isSolid, float x, float y) {
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
    solid = isSolid;
}

// Draw the tile on the screen
void Tile::draw(sf::RenderWindow &window) {
    window.draw(sprite);
}

// Check if the tile is solid (collision detection)
bool Tile::isSolid() const {
    return solid;
}

// Get the bounding box of the tile (for collision)
sf::FloatRect Tile::getBounds() const {
    return sprite.getGlobalBounds();
}
