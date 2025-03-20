#include "Headers/enemy.hpp"
#include "Headers/mouvement.hpp"
#include "Headers/player.hpp"
#include "Headers/friendlymushroom.hpp"
#include <iostream>


FriendlyMushroom::FriendlyMushroom(float x, float y)
    : Enemy("images/champignon.png", x, y, 0, 0),  // Set limits to 0 in parent class
      mouvement(sprite, 0.5f)  // Adjust speed as needed
{
    sprite.setScale(0.1f, 0.1f);
    movingRight = true;  // Start moving right
    std::cout << "FriendlyMushroom created at position (" << x << ", " << y << ")" << std::endl;
}

void FriendlyMushroom::update() 
{
    // Only process if alive
    if (!alive) return;
    
    // Store current position
    sf::Vector2f oldPosition = sprite.getPosition();

    // Move based on direction without boundary checks
    if (movingRight) {
        mouvement.moveRight();
    } else {
        mouvement.moveLeft();
    }

    // Let the mushroom move continuously in one direction
    // You might want to add screen boundary checks or other logic here
    
    // Debug info
    sf::Vector2f newPosition = sprite.getPosition();
    if (oldPosition.x == newPosition.x) {
        std::cout << "Warning: Mushroom didn't move!" << std::endl;
    }
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

