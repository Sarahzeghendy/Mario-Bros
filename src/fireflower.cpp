#include "Headers/fireflower.hpp"
#include <iostream>

FireFlower::FireFlower(float x, float y)
    : collected(false)
{
    if (!texture.loadFromFile("images/fireflower.png"))
    {
        throw std::runtime_error("Erreur : Impossible de charger fireflower.png");
    }

    sprite.setTexture(texture);
    sprite.setPosition(x, y);
    sprite.setScale(0.1f, 0.1f); 
}

void FireFlower::draw(sf::RenderWindow &window)
{
    if (!collected)
    {
        window.draw(sprite);
    }
}

sf::FloatRect FireFlower::getBounds() const
{
    return sprite.getGlobalBounds();
}

bool FireFlower::isCollected() const
{
    return collected;
}

void FireFlower::collect()
{
    collected = true;
    std::cout << "Mario a collecté une Fleur de Feu !" << std::endl;
}
