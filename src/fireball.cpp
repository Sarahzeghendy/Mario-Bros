#include "Headers/fireball.hpp"
#include <iostream>

Fireball::Fireball(float x, float y, float direction)
    : speed(4.0f * direction), gravity(0.2f), velocityY(0.0f), active(true), initialX(x)
{
    if (!texture.loadFromFile("images/fireball.png"))
    {
        throw std::runtime_error("Erreur : Impossible de charger fireball.png");
    }

    sprite.setTexture(texture);
    sprite.setPosition(x, y);
    sprite.setScale(0.03f, 0.03f);
}

void Fireball::update(const std::vector<sf::Sprite> &blocks)
{
    if (!active) return;
 
    sprite.rotate(10.0f);
  
    sprite.move(speed, velocityY);

    velocityY += gravity;

    if (velocityY > 5.0f) {
        velocityY = 5.0f;
    }
    

    for (const auto& block : blocks) {
        if (sprite.getGlobalBounds().intersects(block.getGlobalBounds())) {
         
            if (velocityY > 0) {
                velocityY = -4.0f; 
            } 
            else {
                active = false;
                return;
            }
        }
    }

    
    if (std::abs(sprite.getPosition().x - initialX) > 1000) {
        active = false;
    }

    if (sprite.getPosition().y > 800) {
        active = false;
    }
}

void Fireball::draw(sf::RenderWindow &window)
{
    if (active)
    {
        window.draw(sprite);
    }
}

sf::FloatRect Fireball::getBounds() const
{
    return sprite.getGlobalBounds();
}

bool Fireball::isActive() const
{
    return active;
}

void Fireball::destroy()
{
    active = false;
}
