#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <random>
#include "player.hpp"
#include "enemy.hpp"

class AIPlayer {
public:
    AIPlayer(Player* character, const std::vector<std::unique_ptr<Enemy>>& enemies);
    void update(const std::vector<sf::Sprite>& groundTiles, const std::vector<sf::Sprite>& pipes, const sf::Sprite& flag);
    void updateEnemyReferences(std::vector<std::unique_ptr<Enemy>>& newEnemies);
    
private:
    void makeDecision();
    void executeAction();
    void avoidEnemies();
    void avoidObstacles(const std::vector<sf::Sprite>& obstacles, const std::vector<sf::Sprite>& pipes);
    void collectPowerups();
    void jumpOverGaps();
    bool detectGapAhead(const std::vector<sf::Sprite>& groundTiles);
    bool isEnemyNearby(float distance);
    void moveTowardsFlag(const sf::Sprite& flag);
    
    Player* character;
    std::vector<const Enemy*> enemies;
    
    std::mt19937 rng;
    int decisionCooldown;
    std::string currentAction;
    int actionDuration;
    float jumpTimer;

    bool shouldJump;
    bool shouldMoveRight;
    bool shouldMoveLeft;
    bool shouldShootFireball;
 
    const float enemyDetectionRange = 200.0f;
    const float gapDetectionRange = 100.0f;
    const int minActionDuration = 30;
    const int maxActionDuration = 90;
};
