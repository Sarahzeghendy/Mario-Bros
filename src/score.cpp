#include "Headers/score.hpp"
#include <iostream>

/**
 * @brief Constructeur de la classe Score
 * @param iconPath Chemin de l'icone du score
 * @param font Police de caracteres pour le score
 * @param isLivesDisplay Indique si le score est un affichage de vies
 * @details Ce constructeur initialise l'icone, le texte et les coeurs du score
 */
Score::Score(const std::string &iconPath, const sf::Font &font, bool isLivesDisplay)
    : font(font), isLivesDisplay(isLivesDisplay)
{
    // Charger la texture de l'icone
    if (!iconTexture.loadFromFile(iconPath))
    {
        std::cerr << "Error loading icon: " << iconPath << std::endl;
    }

    icon.setTexture(iconTexture);
    icon.setScale(0.5f, 0.5f);

    if (!isLivesDisplay)
    {
        // Initialiser le texte du score
        scoreText.setFont(font);
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setString(": 0");
    }
    else
    {
        // Charger la texture des coeurs
        if (!heartTexture.loadFromFile("images/heart.png"))
        {
            std::cerr << "Error loading heart icon: images/heart.png" << std::endl;
        }

        // Creer les sprites pour les coeurs
        for (int i = 0; i < MAX_HEARTS; i++)
        {
            sf::Sprite heartSprite;
            heartSprite.setTexture(heartTexture);
            heartSprite.setScale(0.1f, 0.1f);
            hearts.push_back(heartSprite);
        }
    }
}

/**
 * @brief Definit la position du score
 * @param x Position horizontale
 * @param y Position verticale
 */
void Score::setPosition(float x, float y)
{
    // Positionner l'icone
    icon.setPosition(x, y);

    if (!isLivesDisplay)
    {
        // Positionner le texte du score
        scoreText.setPosition(x + icon.getGlobalBounds().width + 10, y);
    }
    else
    {
        // Positionner les coeurs
        float heartX = x + icon.getGlobalBounds().width + 10;
        for (int i = 0; i < MAX_HEARTS; i++)
        {
            hearts[i].setPosition(heartX + i * 30, y + 8);
        }
    }
}

/**
 * @brief Met a jour le score
 * @param value Nouvelle valeur du score ou des vies
 */
void Score::update(int value)
{
    if (!isLivesDisplay)
    {
        // Mettre a jour le texte du score
        scoreText.setString(": " + std::to_string(value));
    }
    else
    {
        // Mettre a jour l'affichage des coeurs
        for (int i = 0; i < MAX_HEARTS; i++)
        {
            if (i < value)
            {
                hearts[i].setColor(sf::Color::White);
            }
            else
            {
                hearts[i].setColor(sf::Color(255, 255, 255, 0));
            }
        }
    }
}

/**
 * @brief Dessine le score
 * @param window Fenetre de rendu
 */
void Score::draw(sf::RenderWindow &window)
{
    // Dessiner l'icone
    window.draw(icon);

    if (!isLivesDisplay)
    {
        // Dessiner le texte du score
        window.draw(scoreText);
    }
    else
    {
        // Dessiner les coeurs
        for (auto &heart : hearts)
        {
            window.draw(heart);
        }
    }
}
