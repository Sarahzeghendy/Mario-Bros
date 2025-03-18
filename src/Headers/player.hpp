#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include "mouvement.hpp"
#include "fireball.hpp"
#include <vector>

class Enemy;  

class Player  {
private:
    sf::Texture normalTexture; 
    sf::Texture fireTexture;
    sf::Sprite sprite;
    std::string characterName; // Add this to track which character (Mario or Luigi)
    Mouvement mouvement;
    sf::Keyboard::Key rightKey;
    sf::Keyboard::Key leftKey;
    sf::Keyboard::Key jumpKey;
    //bool big;  
    bool isDead;
    bool movingRight;
    bool movingLeft;
    int currentFrame;
    int frameCounter;
    std::string characterType;


    // Constructor
    //Player(const std::string& texturePath, float x, float y, float speed,sf::Keyboard::Key right, sf::Keyboard::Key left, sf::Keyboard::Key jump,const std::string& characterType );
    

public:
  
    Player(const std::string& texturePath, const std::string& name, float x, float y, float speed, 
           sf::Keyboard::Key right, sf::Keyboard::Key left, sf::Keyboard::Key jump);

    bool big; 
    void update(const std::vector<sf::Sprite>& blocks, const std::vector<sf::Sprite>& pipes);
    void jump();
    void applyGravity(const std::vector<sf::Sprite>& blocks, const std::vector<sf::Sprite>& pipes);
    void draw(sf::RenderWindow& window);
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
    bool hasFirePower;
    int lives;
    int coins;
    int fireballCooldown;
    std::vector<Fireball> fireballs;

    sf::Sprite& getSprite();

    bool checkWin(const sf::Sprite& flag);

    void collectFireFlower();
    void shootFireball();
    void updateFireballs(const std::vector<sf::Sprite>& blocks);
    void drawFireballs(sf::RenderWindow &window);

    std::vector<Fireball>& getFireballs() { return fireballs; }
    bool hasFirePowerActive() const { return hasFirePower; }

  
    void loseFirePower();
   
    void bounce() { mouvement.bounce(); }


};

#endif