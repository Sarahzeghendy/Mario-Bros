#include "Headers/enemy.hpp"
#include <iostream>

KoopaTroopa::KoopaTroopa(float x, float y)
    : Enemy("images/koopa.png", x, y, 0, 0)  
{
    sprite.setScale(0.15f, 0.15f);
    speed = 0.25f;  // Increased from 0.1f
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

    sf::Vector2f oldPosition = sprite.getPosition();

    if (movingRight) {
        if (inShellState) {
            sprite.move(speed * 3, 0);  
        } else {
            sprite.move(speed, 0);
        }

        if (!inShellState && sprite.getScale().x < 0) {
            sprite.setScale(0.15f, 0.15f);
        }
    } else {
        if (inShellState) {
            sprite.move(-speed * 3, 0);  
        } else {
            sprite.move(-speed, 0);
        }

        if (!inShellState && sprite.getScale().x > 0) {
            sprite.setScale(-0.15f, 0.15f);
        }
    }

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
    if (inShellState) {
        movingRight = !movingRight;
   
        sf::Vector2f currentPos = sprite.getPosition();
        if (movingRight) {
            sprite.setPosition(currentPos.x + 10.0f, currentPos.y);
        } else {
            sprite.setPosition(currentPos.x - 10.0f, currentPos.y);
        }
     
        speed = 0.7f; // Increased from 0.35f for shell movement
        std::cout << "Koopa shell kicked in other direction with boost!" << std::endl;
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
    
    movingRight = (rand() % 2 == 0);  
    
    speed = 0.8f;  // Increased from 0.4f for shell initial speed
    
}

void KoopaTroopa::onFireballHit()
{
   
    alive = false;
    std::cout << "Koopa defeated by fireball!" << std::endl;
    
    sprite.setScale(0.15f, -0.15f);  
}

void KoopaTroopa::reverseDirection() {
    movingRight = !movingRight;
    
    if (movingRight) {
        sprite.setScale(0.15f, 0.15f);  
    } else {
        sprite.setScale(-0.15f, 0.15f); 
    }
}

void KoopaTroopa::handleCollisions(const sf::Vector2f& oldPosition) 
{
    if (!inShellState) {
        Enemy::handleCollisions(oldPosition);
        return;
    }

    sf::FloatRect globalBounds = sprite.getGlobalBounds();
    
  
    sf::FloatRect rightBounds(
        globalBounds.left + globalBounds.width - 5, 
        globalBounds.top, 
        10, 
        globalBounds.height - 5 
    );
    
    sf::FloatRect leftBounds(
        globalBounds.left - 5, 
        globalBounds.top, 
        10, 
        globalBounds.height - 5 
    );
    
    bool collided = false;
 
    for (const auto& pipe : currentPipes) {
        sf::FloatRect pipeBounds = pipe.getGlobalBounds();
        float pipeLeft = pipeBounds.left + 60.0f;
        float pipeWidth = 35.4f;
        sf::FloatRect fixedPipeBounds(pipeLeft, pipeBounds.top, pipeWidth, pipeBounds.height);
        
        if (movingRight && rightBounds.intersects(fixedPipeBounds)) {
            collided = true;
            break;
        }
        else if (!movingRight && leftBounds.intersects(fixedPipeBounds)) {
            collided = true;
            break;
        }
    }

    if (!collided) {
        for (const auto& block : currentBlocks) {
            sf::FloatRect blockBounds = block.getGlobalBounds();
            
            if (blockBounds.top > globalBounds.top + (globalBounds.height * 0.8f)) {
                continue; 
            }
            
            if (movingRight && rightBounds.intersects(blockBounds)) {
                collided = true;
                break;
            }
            else if (!movingRight && leftBounds.intersects(blockBounds)) {
                collided = true;
                break;
            }
        }
    }
    
    if (collided) {
        movingRight = !movingRight;
     
        if (movingRight) {
            sprite.setPosition(oldPosition.x - 15.0f, oldPosition.y); 
        } else {
            sprite.setPosition(oldPosition.x + 15.0f, oldPosition.y);
        }
        
       
    }
}
