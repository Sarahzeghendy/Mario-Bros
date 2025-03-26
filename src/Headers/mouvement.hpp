#ifndef MOUVEMENT_HPP
#define MOUVEMENT_HPP

#include <SFML/Graphics.hpp>

class Mouvement
{
private:
    sf::Sprite &sprite;
    float speed;
    bool isJumping;
    float velocityY;
    float gravity;
    float jumpStrength;
    float groundY;
    float hangTime;
    float hangCounter;

public:
    Mouvement(sf::Sprite &sprite, float speed);
    void moveRight();
    void moveLeft();
    void jump();
    void applyGravity(const std::vector<sf::Sprite> &blocks, const std::vector<sf::Sprite> &pipes, const std::vector<sf::Sprite> &questionBlocks);                      
    std::pair<bool, bool> blockMovement(const std::vector<sf::Sprite> &obstacles, const std::vector<sf::Sprite> &pipes, const std::vector<sf::Sprite> &questionBlocks); 
    void checkForGaps(const std::vector<sf::FloatRect> &gaps);
    void setSpeed(float newSpeed)
    {
        speed = newSpeed;
    }

    void bounce()
    {
        velocityY = jumpStrength * 0.6f;
        isJumping = true;
    }
    bool getIsJumping() const { return isJumping; }
};

#endif
