#include "game.hpp"
#include <iostream>


void Game::handleWin(int playerId) {
    gameOver = true;
    winner = playerId;
}

void Game::drawResult(sf::RenderWindow& window) {
    if (gameOver) {
        sf::Font font;

        if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf")) {
            throw std::runtime_error("Erreur : Impossible de charger la police par défaut");
        }

        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(50);
        text.setFillColor(sf::Color::White);

        if (winner == 1) {
            text.setString("Mario wins !");
        } else if (winner == 2) {
            text.setString("Luigi wins !");
        }

        text.setPosition(400, 300);
        window.draw(text);
    }
}
