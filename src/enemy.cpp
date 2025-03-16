#include "Headers/enemy.hpp"
#include "Headers/mouvement.hpp"
#include "Headers/player.hpp"
#include <iostream>

Enemy::Enemy(const std::string& imagePath, float x, float y, float leftLim, float rightLim) 
    : texture(),
      sprite(),
      alive(true), 
      movingRight(true),
      mouvement(sprite, 0.03f),
      leftLimit(leftLim), 
      rightLimit(rightLim),
      isFalling(false),
      fallSpeed(0.0f),
      GRAVITY(0.5f)
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

void Enemy::onFireballHit()
{
    alive = false;
    std::cout << "Enemy defeated by fireball!" << std::endl;
}

void Enemy::checkForGaps(const std::vector<sf::FloatRect>& gaps)
{
    if (!alive || isFalling) return;

    sf::FloatRect feetArea = sprite.getGlobalBounds();
    feetArea.top += feetArea.height;
    feetArea.height = 5.0f; 
    
    for (const auto& gap : gaps)
    {
        if (feetArea.intersects(gap))
        {
            isFalling = true;
            fallSpeed = 0.0f;
            std::cout << "Enemy fell into a gap!" << std::endl;
            return;
        }
    }
}

void Enemy::fall()
{
    if (isFalling)
    {
        fallSpeed += GRAVITY;
        sprite.move(0, fallSpeed);
   
        if (sprite.getPosition().y > 800)
        {
            alive = false;
            std::cout << "Enemy fell out of the world!" << std::endl;
        }
    }
}
