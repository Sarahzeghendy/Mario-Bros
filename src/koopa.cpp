#include "Headers/enemy.hpp"
#include <iostream>

KoopaTroopa::KoopaTroopa(float x, float y, float leftLim, float rightLim)
    : Enemy("images/koopa.png", x, y, leftLim, rightLim) 
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

    // Move based on direction
    if (movingRight) {
        sprite.move(speed, 0);
        if (sprite.getPosition().x >= rightLimit) {
            movingRight = false;
            sprite.setScale(-0.15f, 0.15f);  
        }
    } else {
        sprite.move(-speed, 0);
        if (sprite.getPosition().x <= leftLimit) {
            movingRight = true;
            sprite.setScale(0.15f, 0.15f);  
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

    inShellState = true;

    sf::Vector2f originalPosition = sprite.getPosition();
    
    sprite.setTexture(koopaShell, true);  

    sf::Vector2u textureSize = koopaShell.getSize();
    sprite.setTextureRect(sf::IntRect(0, 0, textureSize.x, textureSize.y));
    
    sprite.setPosition(originalPosition);
    sprite.setScale(0.15f, 0.15f);
    
    movingRight = (rand() % 2 == 0);  
    mouvement.setSpeed(0.06f);  
    
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
}
