#pragma once
#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>

class Game {
private:
    bool aiDead_;   
    bool isOver;
    int winner;
    sf::Font font;
    std::vector<sf::Text> results;

public:
    Game();
    
    void handleWin(int playerId);
    void drawResult(sf::RenderWindow& window);
    void handleAIDeath();
    bool isAIDead() const { return aiDead_; }

    bool isGameOver() const { return isOver; } 
};

#endif
