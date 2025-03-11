#include "Headers/enemy.hpp"
#include "Headers/mouvement.hpp"
#include "Headers/player.hpp"
#include <iostream>

Enemy::Enemy(const std::string& imagePath, float x, float y, float leftLim, float rightLim) 
    : alive(true), movingRight(true), leftLimit(leftLim), rightLimit(rightLim), 
      mouvement(sprite, 0.03f) // Change 0.3f to adjust speed for all enemies
{
    if (!texture.loadFromFile(imagePath)) 
    {
        std::cerr << "Erreur: Impossible de charger l'image " << imagePath << "\n";
    }
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
    sprite.setScale(0.1f, 0.1f);
}

void Enemy::render(sf::RenderWindow& window) 
{
    if (alive) {
        window.draw(sprite);
    }
}

bool Enemy::isAlive() const 
{
    return alive;
}

sf::FloatRect Enemy::getBounds() const 
{
    return sprite.getGlobalBounds();
}

void Enemy::draw(sf::RenderWindow& window) 
{
    window.draw(sprite);
}
