#include "Headers/enemy.hpp"
#include <iostream>

KoopaTroopa::KoopaTroopa(float x, float y)
    : Enemy("images/koopa.png", x, y, 0, 0)  // Set limits to 0 in parent class
{
    sprite.setScale(0.15f, 0.15f);
    speed = 0.03f;  // Set the movement speed
    if (!koopaShell.loadFromFile("images/koopa_closed.png")) {
        std::cerr << "Erreur: Impossible de charger koopa_closed.png\n";
    }
}

void KoopaTroopa::update() 
{
    if (!alive) return;
    if (isFalling)
    {
        fall();
        return;
    }

    // Store current position
    sf::Vector2f oldPosition = sprite.getPosition();

    // Move based on direction without checking limits
    if (movingRight) {
        if (inShellState) {
            // Use the faster shell speed when in shell state
            sprite.move(speed * 2, 0);
        } else {
            sprite.move(speed, 0);
        }
        
        // Keep sprite facing right (only if not in shell state)
        if (!inShellState && sprite.getScale().x < 0) {
            sprite.setScale(0.15f, 0.15f);
        }
    } else {
        if (inShellState) {
            // Use the faster shell speed when in shell state
            sprite.move(-speed * 2, 0);
        } else {
            sprite.move(-speed, 0);
        }
        
        // Keep sprite facing left (only if not in shell state)
        if (!inShellState && sprite.getScale().x > 0) {
            sprite.setScale(-0.15f, 0.15f);
        }
    }

    // Use the generic collision handling method
    handleCollisions(oldPosition);
}

void KoopaTroopa::interactWithPlayer(Player& player) 
{
    if (sprite.getGlobalBounds().intersects(player.getbounds())) 
    {
        
        if (inShellState) {
            std::cout << "Le joueur touche la carapace sans danger" << std::endl;
            return;
        }

        else if (player.isBig()) {
            
            player.shrink();
            std::cout << "Mario devient petit après avoir touché un Koopa!" << std::endl;
        } 
        else {
           
            player.die();
            std::cout << "Mario est tué par le Koopa!" << std::endl;
        }
    }
}

void KoopaTroopa::onJumpedOn() 
{
    // If already in shell state, just reverse direction
    if (inShellState) {
        movingRight = !movingRight;
        std::cout << "Koopa shell kicked in other direction!" << std::endl;
        return;
    }
    
    inShellState = true;

    sf::Vector2f originalPosition = sprite.getPosition();
    originalPosition.y -= 5.0f;
    
    sprite.setTexture(koopaShell, true);  

    sf::Vector2u textureSize = koopaShell.getSize();
    sprite.setTextureRect(sf::IntRect(0, 0, textureSize.x, textureSize.y));
    
    sprite.setPosition(originalPosition);
    sprite.setScale(0.12f, 0.12f);
    
    // Don't change direction when first entered shell state
    // movingRight = (rand() % 2 == 0);  
    
    // Increase the speed for shell state
    speed = 0.06f;  
    
    std::cout << "Koopa dans sa carapace!" << std::endl;
}

void KoopaTroopa::onFireballHit()
{
   
    alive = false;
    std::cout << "Koopa defeated by fireball!" << std::endl;
    
    sprite.setScale(0.15f, -0.15f);  
}

void KoopaTroopa::reverseDirection() {
    movingRight = !movingRight;
    
    // Update sprite direction when reversing
    if (movingRight) {
        sprite.setScale(0.15f, 0.15f);  // Face right
    } else {
        sprite.setScale(-0.15f, 0.15f); // Face left
    }
}
