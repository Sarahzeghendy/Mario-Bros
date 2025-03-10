#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include "mouvement.hpp"

class Enemy;  // Forward declaration

class Player  {
private:
    sf::Texture texture;
    sf::Sprite sprite;
    Mouvement mouvement;
    sf::Keyboard::Key leftKey;
    sf::Keyboard::Key rightKey;
    sf::Keyboard::Key jumpKey;
    int lives;
    int coins;
    bool isDead;
    bool movingRight;
    bool movingLeft;

public:
    // Constructor
    Player(const std::string& texturePath, float x, float y, float speed,sf::Keyboard::Key right, sf::Keyboard::Key left, sf::Keyboard::Key jump);

    bool big;  

   
    void update(const std::vector<sf::Sprite>& blocks, const std::vector<sf::Sprite>& pipes);
    void jump();
    void applyGravity(const std::vector<sf::Sprite>& blocks, const std::vector<sf::Sprite>& pipes);
    void draw(sf::RenderWindow& window);

    // Movement related
    void moveRight() { mouvement.moveRight(); }
    void moveLeft() { mouvement.moveLeft(); }
    bool isMovingRight() const { return movingRight; }
    bool isMovingLeft() const { return movingLeft; }

    // Position and bounds
    sf::Vector2f getPosition() const { return sprite.getPosition(); }
    void setPosition(float x, float y) { sprite.setPosition(x, y); }
    sf::FloatRect getbounds() const { return sprite.getGlobalBounds(); }

    // Game mechanics
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
};

#endif 