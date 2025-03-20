#include "Headers/AIPlayer.hpp"
#include <iostream>
#include <cmath>
#include <algorithm>

AIPlayer::AIPlayer(Player* character, const std::vector<std::unique_ptr<Enemy>>& enemies)
    : character(character), 
      decisionCooldown(0), 
      currentAction("idle"), 
      actionDuration(0),
      jumpTimer(0),
      shouldJump(false),
      shouldMoveRight(true),
      shouldMoveLeft(false),
      shouldShootFireball(false)
{
    for (const auto& enemy : enemies) {
        this->enemies.push_back(enemy.get());
    }

    std::random_device rd;
    rng = std::mt19937(rd());
}

void AIPlayer::update(const std::vector<sf::Sprite>& groundTiles, 
                      const std::vector<sf::Sprite>& pipes,
                      const sf::Sprite& flag)
{
   
    if (decisionCooldown > 0) {
        decisionCooldown--;
    }

    if (decisionCooldown <= 0) {
        makeDecision();
    }

    avoidEnemies();
    avoidObstacles(groundTiles, pipes);

    moveTowardsFlag(flag);

    if (detectGapAhead(groundTiles)) {
        shouldJump = true;
        shouldMoveRight = true;
    }

    executeAction();

    if (jumpTimer > 0) {
        jumpTimer--;
        shouldJump = (jumpTimer > 0);
    }
 
    if (character->hasFirePowerActive() && std::bernoulli_distribution(0.01)(rng)) {
        shouldShootFireball = true;
    }
}

void AIPlayer::makeDecision()
{
    
    std::vector<std::string> actions = {"move_right", "move_left", "jump", "idle"};
    std::vector<double> weights = {0.6, 0.1, 0.2, 0.1}; 
    
    std::discrete_distribution<> d(weights.begin(), weights.end());
    currentAction = actions[d(rng)];
    
    actionDuration = std::uniform_int_distribution<>(minActionDuration, maxActionDuration)(rng);
    decisionCooldown = actionDuration;

    shouldMoveRight = (currentAction == "move_right");
    shouldMoveLeft = (currentAction == "move_left");
    shouldJump = (currentAction == "jump");

    std::cout << "AI Decision: " << currentAction << " for " << actionDuration << " frames" << std::endl;
}

void AIPlayer::executeAction()
{
    
    if (shouldJump && !character->isInAir()) {
        character->jump();
        std::cout << "Luigi AI executed jump!" << std::endl;
        jumpTimer = jumpTimer > 0 ? jumpTimer : 10;
    }
    
    character->setMovingRight(shouldMoveRight);
    character->setMovingLeft(shouldMoveLeft);
    
    std::cout << "Luigi position: (" << character->getPosition().x << ", " 
              << character->getPosition().y << ") right=" 
              << (shouldMoveRight ? "true" : "false")
              << " left=" << (shouldMoveLeft ? "true" : "false") 
              << std::endl;

    if (shouldShootFireball) {
        character->shootFireball();
        shouldShootFireball = false; 
    }
}

void AIPlayer::avoidEnemies()
{
    bool enemyDetected = false;
    
   
    sf::Vector2f originalPosition = character->getPosition();
    sf::FloatRect characterBounds = character->getbounds();

    float detectionRange = character->isSmall() ? enemyDetectionRange * 1.5f : enemyDetectionRange;
    
    for (const auto enemy : enemies) {
        if (!enemy->isAlive()) continue;
        
        sf::FloatRect enemyBounds = enemy->getBounds();
        float distanceX = enemyBounds.left - characterBounds.left;
        float distanceY = enemyBounds.top - characterBounds.top;
        float distance = std::sqrt(distanceX * distanceX + distanceY * distanceY);
        

        if (distance < detectionRange) {
            enemyDetected = true;
  
            if (distanceX > 0 && distanceX < 150) {
               
                if (std::abs(distanceY) < 80) {
               
                    if (distanceX < 80 && !character->isInAir() && jumpTimer <= 0) {
                        shouldJump = true;
                        jumpTimer = 20; 
                    }
      
                    if (character->hasFirePowerActive()) {
                        shouldShootFireball = true;
                    }
          
                    if (std::bernoulli_distribution(0.4)(rng)) {
                        shouldMoveLeft = true;
                        shouldMoveRight = false;
                    }
                }
            } 
     
            else if (distanceX < 0 && distanceX > -150) {
            
                if (std::abs(distanceY) < 80) {
                 
                    if (distanceX > -80 && !character->isInAir() && jumpTimer <= 0) {
                        shouldJump = true;
                        jumpTimer = 20; 
                    }
            
                    shouldMoveRight = true;
                    shouldMoveLeft = false;
         
                    if (character->hasFirePowerActive()) {
                        shouldShootFireball = true;
                    }
                }
            }
           
            if (distance < 50 && !character->isInAir()) {
                shouldJump = true;
                jumpTimer = 25; 
       
                shouldMoveRight = (distanceX < 0); 
                shouldMoveLeft = (distanceX > 0);  
            }
        }
    }
    
    if (enemyDetected && !character->isInAir() && std::bernoulli_distribution(0.3)(rng)) {
        shouldJump = true;
        jumpTimer = 15;
    }
    

    if (character->getPosition().y > 600 || character->getPosition().y < 0) {
      
        character->setPosition(character->getPosition().x, originalPosition.y);
    }

    sf::Color currentColor = character->getSprite().getColor();
    if (currentColor.a < 255) {
        sf::Color newColor = currentColor;
        newColor.a = 255;
        character->getSprite().setColor(newColor);
    }
}

void AIPlayer::avoidObstacles(const std::vector<sf::Sprite>& obstacles, const std::vector<sf::Sprite>& pipes)
{
    sf::FloatRect characterBounds = character->getbounds();

    sf::FloatRect aheadBounds = characterBounds;
    aheadBounds.left += (shouldMoveRight ? 50.0f : -50.0f);
  
    for (const auto& obstacle : obstacles) {
        if (aheadBounds.intersects(obstacle.getGlobalBounds())) {
            shouldJump = true;
            jumpTimer = 15;
            break;
        }
    }
 
    for (const auto& pipe : pipes) {
        if (aheadBounds.intersects(pipe.getGlobalBounds())) {
            shouldJump = true;
            jumpTimer = 15;
            break;
        }
    }
}

bool AIPlayer::detectGapAhead(const std::vector<sf::Sprite>& groundTiles)
{
    sf::Vector2f pos = character->getPosition();
    float charBottom = pos.y + character->getbounds().height;
    float checkDistance = gapDetectionRange;

    sf::Vector2f checkPoint(pos.x + (shouldMoveRight ? checkDistance : -checkDistance), charBottom + 30); 
    bool groundFound = false;
    for (const auto& tile : groundTiles) {
        sf::FloatRect tileBounds = tile.getGlobalBounds();
        if (checkPoint.x >= tileBounds.left && checkPoint.x <= tileBounds.left + tileBounds.width &&
            std::abs(checkPoint.y - tileBounds.top) < 50) { 
            groundFound = true;
            break;
        }
    }

    if (!groundFound) {
        std::cout << "Gap detected ahead! Jumping!" << std::endl;
        return true;
    }
    
    return false;
}


bool AIPlayer::isEnemyNearby(float distance)
{
    for (const auto enemy : enemies) {
        if (!enemy->isAlive()) continue;
        
        float distanceX = std::abs(enemy->getBounds().left - character->getPosition().x);
        if (distanceX < distance) {
            return true;
        }
    }
    return false;
}

void AIPlayer::moveTowardsFlag(const sf::Sprite& flag)
{
  
    float flagX = flag.getPosition().x;
    float characterX = character->getPosition().x;
    float distanceToFlag = flagX - characterX;
 
    if (distanceToFlag > 0) {
        if (std::bernoulli_distribution(0.8)(rng)) {
            shouldMoveRight = true;
            shouldMoveLeft = false;
        }
    }
}

/**
 * @brief Updates the AI's references to the game's enemies.
 * @param newEnemies Vector containing pointers to all active enemies.
 */
void AIPlayer::updateEnemyReferences(std::vector<std::unique_ptr<Enemy>>& newEnemies) {
   
    enemies.clear();
    
    for (const auto& enemy : newEnemies) {
        enemies.push_back(enemy.get());
    }
    
    std::cout << "AI updated with " << enemies.size() << " enemies to track" << std::endl;
}
