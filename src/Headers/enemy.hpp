#ifndef ENEMY_HPP
#define ENEMY_HPP

class Player; //déclaration aticipée de la classe Player

#include <SFML/Graphics.hpp>
#include "player.hpp"
#include "mouvement.hpp"
#include <iostream>


class Enemy 
{
    public:
        Enemy(const std::string& imagePath,float x, float y, float leftLim, float rightLim);  
        virtual ~Enemy() = default;

        virtual void update() = 0;
        virtual void interactWithPlayer(Player& player);
        virtual void onJumpedOn() = 0;
        virtual void onFireballHit(); 
        virtual void checkForGaps(const std::vector<sf::FloatRect>& gaps); 
        void fall(); 
   
        void render(sf::RenderWindow& window); 
        bool isAlive() const;     
        sf::FloatRect getBounds() const; 
        void draw(sf::RenderWindow& window);

        void setCurrentLevel(const std::vector<sf::Sprite>& blocks, 
                             const std::vector<sf::Sprite>& pipes,
                             const std::vector<Enemy*>& enemies);

        virtual void reverseDirection(); 

        void interactWithPlayerSafely(Player& player) {
            auto originalScale = player.getSprite().getScale();
         
            interactWithPlayer(player);
            
            if (player.getSprite().getScale().x < 0.1f || player.getSprite().getScale().y < 0.1f) {
                std::cout << "Warning: Player scale too small after enemy interaction. Restoring." << std::endl;
                player.getSprite().setScale(originalScale);
            }
        }

    protected:
        void handleCollisions(const sf::Vector2f& oldPosition); // New method for collision handling

        sf::Texture texture;
        sf::Sprite sprite;
        bool alive;
        bool movingRight;
        Mouvement mouvement;          
        float leftLimit, rightLimit;  
        bool isFalling = false;        
        float fallSpeed = 0.0f;        
        const float GRAVITY = 0.5f;    
        std::vector<sf::Sprite> currentBlocks;
        std::vector<sf::Sprite> currentPipes;
        std::vector<Enemy*> currentEnemies;
        float speed;
};



class Goomba : public Enemy {
    public:
        Goomba(float x, float y);  // Updated constructor without limits
        void update() override;
        void interactWithPlayer(Player& player) override;
        void onJumpedOn() override;
        void onFireballHit() override;  
        void reverseDirection();
};

class KoopaTroopa : public Enemy {
    public:
        KoopaTroopa(float x, float y);  // Updated constructor without limits
        void update() override;
        void interactWithPlayer(Player& player) override;
        void onJumpedOn() override;
        void onFireballHit() override;
        void reverseDirection();
    
    private:
        sf::Texture koopaShell;  
        bool inShellState = false;  
};

class FriendlyMushroom : public Enemy {
    public:
        FriendlyMushroom(float x, float y);
        void update() override;
        void interactWithPlayer(Player& player) override;
        void onJumpedOn() override; 
        void reverseDirection();
};

#endif
