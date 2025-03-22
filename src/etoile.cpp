#include "Headers/etoile.hpp"
#include <iostream>


Etoile::Etoile(float x, float y)
    : collected(false)
{
    if (!texture.loadFromFile("images/etoile.png"))
    {
        throw std::runtime_error("Erreur : Impossible de charger fireflower.png");
    }

    sprite.setTexture(texture);
    sprite.setPosition(x, y);
    sprite.setScale(0.1f, 0.1f); 
}

void Etoile::draw(sf::RenderWindow &window)
{
    if (!collected)
    {
        window.draw(sprite);
    }
}

sf::FloatRect Etoile::getBounds() const
{
    return sprite.getGlobalBounds();
}

bool Etoile::isCollected() const
{
    return collected;
}

void Etoile::collect()
{
    collected = true;
    std::cout << "Mario a collecté une étoile !" << std::endl;
}
