#pragma once
#include <SFML/Graphics.hpp>

class Game {
private:
    bool gameOver; 
    int winner = -1;
    bool aiDead_;   
    sf::Font font;
    std::vector<sf::Text> results;

public:
    Game();
    
    void handleWin(int playerId);
    void drawResult(sf::RenderWindow& window);
    void handleAIDeath();
    bool isAIDead() const { return aiDead_; }

    bool isGameOver() const { return gameOver; }
};
