#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Mouvement {
    public:
        Mouvement(sf::Sprite &spr, float spd);
        void moveRight();
        void moveLeft();
        void jump();
        void applyGravity(const std::vector<sf::Sprite>& blocks, const std::vector<sf::Sprite>& pipes);
        bool blockMovement(const std::vector<sf::Sprite>& obstacles);
        void checkForGaps(const std::vector<sf::FloatRect>& gaps);
    
    private:
        sf::Sprite &sprite;
        float speed;
        bool isJumping;
        float velocityY;
        float gravity;
        float jumpStrength;
        float hangTime;
        float hangCounter;
    };
    