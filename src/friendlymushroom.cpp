#include "Headers/enemy.hpp"
#include "Headers/mouvement.hpp"
#include "Headers/player.hpp"
#include <iostream>


FriendlyMushroom::FriendlyMushroom(float x, float y)
    : Enemy("images/champignon.png", x, y, 0, 0) // Set limits to 0 in parent class
{
    sprite.setScale(0.1f, 0.1f);
    speed = 1.2f; 
}

void FriendlyMushroom::update() 
{
    // Only process if alive
    if (!alive) return;
    
    // Check if falling first
    if (isFalling)
    {
        fall();
        return;
    }
    
    // Store current position
    sf::Vector2f oldPosition = sprite.getPosition();

    // Move based on direction without checking limits
    if (movingRight) {
        mouvement.moveRight();
    } else {
        mouvement.moveLeft();
    }

    // Use the generic collision handling method
    handleCollisions(oldPosition);
}

void FriendlyMushroom::interactWithPlayer(Player& player) 
{
    if (alive && sprite.getGlobalBounds().intersects(player.getbounds())) 
    {
        if (!player.isBig()) 
        {
            player.grow();
            std::cout << "Le joueur devient grand grâce au champignon gentil!" << std::endl;
            alive = false; // Disparait après avoir été collecté
        }
    }
}


void FriendlyMushroom::onJumpedOn()
{
    if (alive) {
        alive = false;
        std::cout << "Champignon gentil collecté en sautant dessus!" << std::endl;
    }
}

void FriendlyMushroom::reverseDirection() {
    movingRight = !movingRight; 
}