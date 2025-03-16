#ifndef ENEMY_HPP
#define ENEMY_HPP

class Player; //déclaration aticipée de la classe Player

#include <SFML/Graphics.hpp>
#include "player.hpp"
#include "mouvement.hpp"


class Enemy 
{
    public:
        Enemy(const std::string& imagePath,float x, float y, float leftLim, float rightLim);  
        virtual ~Enemy() = default;

        virtual void update() = 0;
        virtual void interactWithPlayer(Player& player) = 0;
        virtual void onJumpedOn() = 0;
        virtual void onFireballHit(); 
        virtual void checkForGaps(const std::vector<sf::FloatRect>& gaps); 
        void fall(); 
   
   
        void render(sf::RenderWindow& window); 
        bool isAlive() const;     
        sf::FloatRect getBounds() const; 
        void draw(sf::RenderWindow& window);

    protected:
        sf::Texture texture;
        sf::Sprite sprite;
        bool alive;
        bool movingRight;
        Mouvement mouvement;          
        float leftLimit, rightLimit;  
        bool isFalling = false;        
        float fallSpeed = 0.0f;        
        const float GRAVITY = 0.5f;    
};



class Goomba : public Enemy {
    public:
        Goomba(float x, float y, float leftLim, float rightLim);
        void update() override;
        void interactWithPlayer(Player& player) override;
        void onJumpedOn() override;
        void onFireballHit() override;  
};

class KoopaTroopa : public Enemy {
    public:
        KoopaTroopa(float x, float y, float leftLim, float rightLim);
        void update() override;
        void interactWithPlayer(Player& player) override;
        void onJumpedOn() override;
        void onFireballHit() override;
    
    private:
        sf::Texture koopaShell;  
        bool inShellState = false;  
};

#endif
