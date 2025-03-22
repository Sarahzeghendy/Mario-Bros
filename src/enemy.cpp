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

void Enemy::setCurrentLevel(const std::vector<sf::Sprite>& blocks, 
                            const std::vector<sf::Sprite>& pipes,
                            const std::vector<Enemy*>& enemies) {
    currentBlocks = blocks;
    currentPipes = pipes;
    currentEnemies = enemies;
}

void Enemy::reverseDirection() {
    movingRight = !movingRight;
}

void Enemy::handleCollisions(const sf::Vector2f& oldPosition) {
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
            // Restore position, reverse direction
            sprite.setPosition(oldPosition.x, oldPosition.y); 
            reverseDirection();
            return; 
        }
        else if (!movingRight && leftBounds.intersects(fixedPipeBounds)) {
            // Restore position, reverse direction
            sprite.setPosition(oldPosition.x, oldPosition.y); 
            reverseDirection();
            return; 
        }
    }

    // Check block collisions
    for (const auto& block : currentBlocks) {
        if (movingRight && rightBounds.intersects(block.getGlobalBounds())) {
            // Restore position, reverse direction
            sprite.setPosition(oldPosition.x, oldPosition.y);
            reverseDirection();
            return; 
        }
        else if (!movingRight && leftBounds.intersects(block.getGlobalBounds())) {
            // Restore position, reverse direction
            sprite.setPosition(oldPosition.x, oldPosition.y); 
            reverseDirection();
            return;  
        }
    }
}

/**
 * @brief Interacts with a player, handling collision detection and effects.
 * @param player The player to interact with.
 */
void Enemy::interactWithPlayer(Player& player) {
    if (player.getIsDead() || !isAlive()) return; // Skip if player is dead or enemy is dead
    
    // Don't interact if player is invincible after a hit
    if (player.isInvincible()) return;
    
    sf::FloatRect playerBounds = player.getbounds();
    sf::FloatRect enemyBounds = sprite.getGlobalBounds();
    
    if (playerBounds.intersects(enemyBounds)) {
        // If player has star power, enemy dies
        if (player.getSpeed() > player.getBaseSpeed()) {
            this->onJumpedOn();
            return;
        }
        
        // If player is big, shrink
        if (player.isBig()) {
            player.shrink();
        } else {
            // If player is small, die
            player.die();
        }
    }
}
