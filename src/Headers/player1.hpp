#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "mouvement1.hpp"

class Player {
public:
    Player(const std::string& texturePath, float x, float y, float speed, 
           sf::Keyboard::Key right, sf::Keyboard::Key left, sf::Keyboard::Key jump);

    void update(const std::vector<sf::Sprite>& blocks, const std::vector<sf::Sprite>& pipes);
    void applyGravity(const std::vector<sf::Sprite>& blocks, const std::vector<sf::Sprite>& pipes);
    void jump();
    void draw(sf::RenderWindow& window);

    void getCoin();    
    void loseLife();   
    void gainLife();  

    void grow();       
    void shrink();   
    bool isBig() const;
    bool isAlive() const;
    
private:
    sf::Sprite sprite;
    sf::Texture texture;

    Mouvement mouvement;

    sf::Keyboard::Key rightKey;
    sf::Keyboard::Key leftKey;
    sf::Keyboard::Key jumpKey;

    int lives;
    int coins;
    bool big;
    bool isDead;
};
