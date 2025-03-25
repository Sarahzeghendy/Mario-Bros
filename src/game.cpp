#include "Headers/game.hpp"
#include <iostream>

/**
 * @brief Constructeur de la classe Game
 */
Game::Game() : aiDead_(false), isOver(false), winner(0)
{
    // Chargement de la police ici pour eviter de la recharger a chaque affichage
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"))
    {
        std::cerr << "Erreur : Impossible de charger la police !" << std::endl;
    }
}

/**
 * @brief Gere la condition de victoire d'un joueur
 * @param playerId Identificateur du joueur gagnant (1 pour Mario, 2 pour Luigi)
 */
void Game::handleWin(int playerId)
{
    isOver = true;     // Marque le jeu comme termine
    winner = playerId; // Enregistre l'identifiant du gagnant
}

/**
 * @brief Affiche le resultat du jeu sur l'ecran
 * @param window Fenetre de rendu
 * @details
 * - Affiche le message de victoire du joueur gagnant
 * - Centrage automatique par rapport a la vue actuelle pour fonctionner
 *   avec une camera mobile
 */
void Game::drawResult(sf::RenderWindow &window)
{
    // Affichage uniquement si le jeu est termine
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

        // Texte different selon le gagnant
        if (winner == 1)
        {
            text.setString("Mario wins !");
        }
        else if (winner == 2)
        {
            text.setString("Luigi wins !");
        }

        // Centrage par rapport a la vue actuelle de la fenetre
        sf::Vector2f viewCenter = window.getView().getCenter();

        // Calcul automatique de l'origine pour centrer le texte
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setOrigin(textBounds.width / 2, textBounds.height / 2);
        text.setPosition(viewCenter.x, viewCenter.y);

        // Fond semi-transparent 
        sf::RectangleShape background(sf::Vector2f(textBounds.width + 40, textBounds.height + 40));
        background.setFillColor(sf::Color(0, 0, 0, 180));
        background.setOrigin(background.getSize().x / 2, background.getSize().y / 2);
        background.setPosition(viewCenter.x, viewCenter.y);

        window.draw(background);
        window.draw(text);
    }

    // Affichage des messages supplementaires (utilise pour l'IA)
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
 * @brief Gestion specifique de la mort du joueur controle par l'IA
* @details Methode dediee a la logique de jeu contre l'IA 
 */
void Game::handleAIDeath()
{
    aiDead_ = true;  // Marque l'IA comme morte
    isOver = true;   // Termine le jeu
    winner = 1;      // Joueur 1 (Mario) gagne

    // Creation d'un message personnalise
    sf::Text aiDeathText;
    aiDeathText.setFont(font);
    aiDeathText.setString("Luigi (AI) a été vaincu!");
    aiDeathText.setCharacterSize(30);
    aiDeathText.setFillColor(sf::Color::Red);

    // Ajout a la liste des messages a afficher
    results.push_back(aiDeathText);
}