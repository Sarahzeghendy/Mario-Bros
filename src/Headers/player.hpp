#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include "mouvement.hpp"
#include "fireball.hpp"
#include <vector>

class Enemy;

class Player
{
private:
    sf::Texture normalTexture;
    sf::Texture fireTexture;
    sf::Sprite sprite;
    std::string characterName;
    Mouvement mouvement;
    sf::Keyboard::Key rightKey;
    sf::Keyboard::Key leftKey;
    sf::Keyboard::Key jumpKey;
    bool big;
    bool isDead;
    bool movingRight;
    bool movingLeft;
    bool hasFirePower;
    int lives;
    int coins;
    int fireballCooldown;
    std::vector<Fireball> fireballs;
    int currentFrame;
    int frameCounter;
    std::string characterType;
    int hitTimer;
    bool isStarPowered = false;
    int starPowerFrames = 0;
    float baseSpeed;
    float currentSpeed;
    int score;
    float initialX;
    float initialY;
    float deathX;
    float deathY;
    int lastLifeThreshold;
    sf::Vector2f previousPosition;

public:
    Player(const std::string &texturePath, const std::string &name, float x, float y, float speed,
           sf::Keyboard::Key right, sf::Keyboard::Key left, sf::Keyboard::Key jump);

    void update(const std::vector<sf::Sprite> &blocks, const std::vector<sf::Sprite> &pipes, const std::vector<sf::Sprite> &questionBlocks); 
    void jump();
    void applyGravity(const std::vector<sf::Sprite> &blocks, const std::vector<sf::Sprite> &pipes, const std::vector<sf::Sprite> &questionBlocks); 
    void draw(sf::RenderWindow &window);
    void animate();

    void moveRight() { mouvement.moveRight(); }
    void moveLeft() { mouvement.moveLeft(); }
    bool isMovingRight() const { return movingRight; }
    bool isMovingLeft() const { return movingLeft; }

    sf::Vector2f getPosition() const { return sprite.getPosition(); }
    void setPosition(float x, float y) { sprite.setPosition(x, y); }
    sf::FloatRect getbounds() const { return sprite.getGlobalBounds(); }

    void getCoins();
    void loseLife();
    void gainLife();
    int getLives() const;

    void grow();
    void shrink();
    bool isBig() const;
    void die();

    bool loadTexture();
    void checkForGaps(const std::vector<sf::FloatRect> &gaps);

    sf::Sprite &getSprite();

    bool checkWin(const sf::Sprite &flag);

    void collectFireFlower();
    void shootFireball();
    void updateFireballs(const std::vector<sf::Sprite> &blocks);
    void drawFireballs(sf::RenderWindow &window);

    std::vector<Fireball> &getFireballs() { return fireballs; }
    bool hasFirePowerActive() const { return hasFirePower; }

    void loseFirePower();

    void bounce() { mouvement.bounce(); }

    int getScore() const { return score; }
    std::string getName() const { return characterName; }

    void setMovingRight(bool move) { movingRight = move; }
    void setMovingLeft(bool move) { movingLeft = move; }
    bool isInAir() const { return mouvement.getIsJumping(); }

    Mouvement &getMouvement() { return mouvement; }

    bool isSmall() const;
    bool isHit() const;
    bool getIsDead() const { return isDead; }
    void setIsDead(bool dead) { isDead = dead; }

    void collectEtoile();
    void setSpeed(float newSpeed) { baseSpeed = newSpeed; }
    float getSpeed() const { return currentSpeed; }
    float getBaseSpeed() const { return baseSpeed; }

    void respawn();
    void updateInitialPosition(float x, float y);
    bool shouldRespawn() const;
    void addScore(int points);

    void updatePreviousPosition() { previousPosition = sprite.getPosition(); }
    sf::Vector2f getPreviousPosition() const { return previousPosition; }
    bool isMoving() const { return sprite.getPosition().x != previousPosition.x; }
};

#endif