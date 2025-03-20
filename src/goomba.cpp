#include "Headers/enemy.hpp"
#include <iostream>

Goomba::Goomba(float x, float y)
    : Enemy("images/goomba.png", x, y, 0, 0)  // Set limits to 0 in parent class
{
}

void Goomba::update() 
{
    if (!alive) return;
    if (isFalling)
    {
        fall();
        return;
    }

    // Store current position
    sf::Vector2f oldPosition = sprite.getPosition();

    // Move based on direction without boundary checks
    if (movingRight) {
        mouvement.moveRight();
    } else {
        mouvement.moveLeft();
    }

    // Handle collisions without checking limits
    handleCollisions(oldPosition);
}

void Goomba::interactWithPlayer(Player& player) 
{
    if (sprite.getGlobalBounds().intersects(player.getbounds())) 
    {


        if (player.hasFirePowerActive()) {
            player.shrink();
            std::cout << "Mario a perdu son pouvoir de feu en touchant un Goomba!" << std::endl;
        }
        else if (player.isBig()) {
            player.shrink();
            std::cout << "Mario devient petit après avoir touché un Goomba!" << std::endl;
        } 
        else {
            player.die();
            std::cout << "Mario est tué par le Goomba!" << std::endl;
        }
    }
}

void Goomba::onJumpedOn() 
{
    alive = false;
    sprite.setScale(0.1f, 0.05f);  // Écrasé plus plat que les autres ennemis
    std::cout << "Goomba écrasé !" << std::endl;
}

void Goomba::onFireballHit() 
{
    alive = false;
    // Add a special death animation when hit by fireball
    sprite.setScale(0.1f, -0.1f);  // Flip upside down
    std::cout << "Goomba touché par une boule de feu et vaincu!" << std::endl;
}

void Goomba::reverseDirection() {
    movingRight = !movingRight;
}
