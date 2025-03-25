#include "Headers/score.hpp"
#include <iostream>

/**
 * @brief Constructeur de la classe Score
 * @param iconPath Chemin de l'icone du score
 * @param font Police de caractères pour le score
 * @param isLivesDisplay Indique si le score est un affichage de vies
 * @details Ce constructeur initialise l'icone, le texte et les coeurs du score
 */
Score::Score(const std::string &iconPath, const sf::Font &font, bool isLivesDisplay)
    : font(font), isLivesDisplay(isLivesDisplay)
{

    if (!iconTexture.loadFromFile(iconPath))
    {
        std::cerr << "Error loading icon: " << iconPath << std::endl;
    }

    icon.setTexture(iconTexture);
    icon.setScale(0.5f, 0.5f);

    if (!isLivesDisplay)
    {
        scoreText.setFont(font);
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setString(": 0");
    }
    else
    {
        if (!heartTexture.loadFromFile("images/heart.png"))
        {
            std::cerr << "Error loading heart icon: images/heart.png" << std::endl;
        }

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
 * @brief Définit la position du score
 * @param x Position horizontale
 * @param y Position verticale
 */
void Score::setPosition(float x, float y)
{
    icon.setPosition(x, y);

    if (!isLivesDisplay)
    {
        scoreText.setPosition(x + icon.getGlobalBounds().width + 10, y);
    }
    else
    {
        float heartX = x + icon.getGlobalBounds().width + 10;
        for (int i = 0; i < MAX_HEARTS; i++)
        {
            hearts[i].setPosition(heartX + i * 30, y + 8);
        }
    }
}

/**
 * @brief Met à jour le score
 */
void Score::update(int value)
{
    if (!isLivesDisplay)
    {
        scoreText.setString(": " + std::to_string(value));
    }
    else
    {
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
 * @param window Fenêtre de rendu
 */
void Score::draw(sf::RenderWindow &window)
{
    window.draw(icon);

    if (!isLivesDisplay)
    {
        window.draw(scoreText);
    }
    else
    {

        for (auto &heart : hearts)
        {
            window.draw(heart);
        }
    }
}
