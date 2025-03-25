#include "Headers/game.hpp"
#include <iostream>

/**
 * @brief Constructeur de la classe Game
 * @details Ce constructeur initialise le jeu avec letat de lIA morte a faux et le jeu termine a faux
 */
Game::Game() : aiDead_(false), isOver(false), winner(0)
{
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"))
    {
        std::cerr << "Erreur : Impossible de charger la police !" << std::endl;
    }
}

/**
 * @brief Met  jour le jeu
 */
void Game::handleWin(int playerId)
{
    isOver = true;
    winner = playerId;
}

/**
 * @brief Affiche le resultat du jeu su lecran
 * @param window Fenetre de rendu
 * @details Cette methode affiche le resultat du jeu sur l ecran
 */
void Game::drawResult(sf::RenderWindow &window)
{
    if (isOver)
    {
        sf::Font font;

        if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf"))
        {
            throw std::runtime_error("Erreur : Impossible de charger la police par défaut");
        }

        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(50);
        text.setFillColor(sf::Color::White);

        if (winner == 1)
        {
            text.setString("Mario wins !");
        }
        else if (winner == 2)
        {
            text.setString("Luigi wins !");
        }

        sf::Vector2f viewCenter = window.getView().getCenter();

        sf::FloatRect textBounds = text.getLocalBounds();
        text.setOrigin(textBounds.width / 2, textBounds.height / 2);
        text.setPosition(viewCenter.x, viewCenter.y);

        sf::RectangleShape background(sf::Vector2f(textBounds.width + 40, textBounds.height + 40));
        background.setFillColor(sf::Color(0, 0, 0, 180));
        background.setOrigin(background.getSize().x / 2, background.getSize().y / 2);
        background.setPosition(viewCenter.x, viewCenter.y);

        window.draw(background);
        window.draw(text);
    }

    for (const auto &resultText : results)
    {
        sf::Vector2f viewCenter = window.getView().getCenter();

        sf::Text centeredText = resultText;
        sf::FloatRect textBounds = centeredText.getLocalBounds();
        centeredText.setOrigin(textBounds.width / 2, textBounds.height / 2);
        centeredText.setPosition(viewCenter.x, viewCenter.y);

        sf::RectangleShape background(sf::Vector2f(textBounds.width + 40, textBounds.height + 40));
        background.setFillColor(sf::Color(0, 0, 0, 180));
        background.setOrigin(background.getSize().x / 2, background.getSize().y / 2);
        background.setPosition(viewCenter.x, viewCenter.y);

        window.draw(background);
        window.draw(centeredText);
    }
}

/**
 * @brief Victoire du joueur IA
 * @details Cette methode est appelee lorsque le joueur IA est vaincu
 * Elle met a jour letat du jeu pour indiquer que le joueur IA est mort
 */
void Game::handleAIDeath()
{
    aiDead_ = true;
    isOver = true;
    winner = 1;

    sf::Text aiDeathText;
    aiDeathText.setFont(font);
    aiDeathText.setString("Luigi (AI) a été vaincu!");
    aiDeathText.setCharacterSize(30);
    aiDeathText.setFillColor(sf::Color::Red);

    results.push_back(aiDeathText);
}