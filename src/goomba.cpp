#include "Headers/enemy.hpp"
#include <iostream>

Goomba::Goomba(float x, float y)
    : Enemy("images/goomba.png", x, y, 0, 0)  // Set limits to 0 in parent class
{
    movingRight = true; // Ensure Goomba starts moving to the right
    speed = 1.0f; // Increase the speed (default was likely around 0.5-1.0)
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

    // Move based on direction and flip the sprite correctly
    if (movingRight) {
        mouvement.moveRight();
        if (sprite.getScale().x > 0) {
            sprite.setScale(-0.1f, 0.1f); // Negative scale faces right
        }
    } else {
        mouvement.moveLeft();
        if (sprite.getScale().x < 0) {
            sprite.setScale(0.1f, 0.1f); // Positive scale faces left
        }
    }

    // Handle collisions without checking limits
    handleCollisionsGoomba(oldPosition);
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

void Goomba::handleCollisionsGoomba(const sf::Vector2f& oldPosition) {
    // Define larger bounding boxes for right and left collision detection
    sf::FloatRect globalBounds = sprite.getGlobalBounds();
    sf::FloatRect rightBounds(globalBounds.left + globalBounds.width - 10, globalBounds.top + 5, 10, globalBounds.height - 10); // Right edge
    sf::FloatRect leftBounds(globalBounds.left, globalBounds.top + 5, 10, globalBounds.height - 10); // Left edge

    // Check pipe collisions
    for (const auto& pipe : currentPipes) {
        sf::FloatRect pipeBounds = pipe.getGlobalBounds();
        float pipeLeft = pipeBounds.left + 60.0f;
        float pipeWidth = 35.4f;
        sf::FloatRect fixedPipeBounds(pipeLeft, pipeBounds.top, pipeWidth, pipeBounds.height);

        if (movingRight && rightBounds.intersects(fixedPipeBounds)) {
            // Restore position, reverse direction, and adjust position
            sprite.setPosition(oldPosition.x - 30.0f, oldPosition.y);
            reverseDirection();
            return; 
        }
        else if (!movingRight && leftBounds.intersects(fixedPipeBounds)) {
            // Restore position, reverse direction, and adjust position slightly
            sprite.setPosition(oldPosition.x + 30.0f, oldPosition.y);
            reverseDirection();
            return; 
        }
    }

    // Check block collisions
    for (const auto& block : currentBlocks) {
        if (movingRight && rightBounds.intersects(block.getGlobalBounds())) {
            // Restore position, reverse direction, and adjust position
            sprite.setPosition(oldPosition.x - 25.0f, oldPosition.y);
            reverseDirection();
            return; 
        }
        else if (!movingRight && leftBounds.intersects(block.getGlobalBounds())) {
            // Restore position, reverse direction, and adjust position
            sprite.setPosition(oldPosition.x + 25.0f, oldPosition.y);
            reverseDirection();
            return;  
        }
    }
}
