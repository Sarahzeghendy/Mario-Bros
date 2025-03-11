#pragma once
#include <SFML/Graphics.hpp>

class Game {
private:
    bool gameOver = false;
    int winner = -1;

public:
    void handleWin(int playerId);
    void drawResult(sf::RenderWindow& window);

    bool isGameOver() const { return gameOver; }
};
