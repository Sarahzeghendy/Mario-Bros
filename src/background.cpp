#include "Headers/background.hpp"
#include <fstream>
#include <iostream>

const float TILE_SIZE = 40.0f;
/**
 * @brief Constructeur de la classe Background
 * @details Ce constructeur initialise les textures des blocs et charge la map du niveau
 * Il initialise également les nuages et le drapeau de fin de niveau
 */
Background::Background()
{

    sky.setSize(sf::Vector2f(10000, 720));
    sky.setPosition(-1000, 0);
    sky.setFillColor(sf::Color(135, 206, 250));

    if (!groundBlock.loadFromFile("images/blocks.png"))
    {
        throw std::runtime_error("Erreur : Impossible de charger blocks.png");
    }
    if (!questionBlockTexture.loadFromFile("images/questionmark_block.png"))
    {
        throw std::runtime_error("Erreur : Impossible de charger questionmark_block.png");
    }
    if (!pipeTexture.loadFromFile("images/pipe.png"))
    {
        throw std::runtime_error("Erreur : Impossible de charger pipe.png");
    }
    if (!cloudTexture.loadFromFile("images/cloud.png"))
    {
        throw std::runtime_error("Erreur : Impossible de charger cloud.png");
    }
    if (!flagTexture.loadFromFile("images/flag.png"))
    {
        throw std::runtime_error("Erreur : Impossible de charger flag.png");
    }

    // load la map du niveau qui est un fichier texte avec chaque caractere representant une partie du niveau
    loadMap("map.txt");

    for (int i = 0; i < 30; i++)
    {
        sf::Sprite cloud(cloudTexture);
        cloud.setPosition(i * 300, 300);
        cloud.setScale(0.5f, 0.5f);
        clouds.push_back(cloud);
    }
}

/**
 * @brief Charge la map du niveau
 * @param filePath Chemin du fichier texte contenant la map du niveau
 * @details Cette methode charge la map du niveau a partir d un fichier texte
 * Chaque caractere du fichier texte represente un pbjet specifique du niveau
 */
void Background::loadMap(const std::string &filePath)
{
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        std::cerr << "Erreur : Impossible de lire le fichier " << filePath << std::endl;
        return;
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line))
    {
        lines.push_back(line);
    }
    file.close();

    const float TILE_SIZE = 40.0f;

    float horizontalOffset = -100.0f;
    float verticalOffset = 50.0f;

    if (verticalOffset < 0)
        verticalOffset = 0;

    for (size_t row = 0; row < lines.size(); row++)
    {
        for (size_t col = 0; col < lines[row].size(); col++)
        {
            char tile = lines[row][col];
            float x = col * TILE_SIZE + horizontalOffset;
            float y = row * TILE_SIZE + verticalOffset;

            if (tile == '#')
            {
                sf::Sprite ground(groundBlock);
                ground.setPosition(x, y);
                groundTiles.push_back(ground);
            }
            else if (tile == 'P' || tile == 'p')
            {
                sf::Sprite pipe(pipeTexture);
                pipe.setPosition(x, y - 17);
                pipe.setScale(0.3f, 0.3f);

                pipes.push_back(pipe);
            }
            else if (tile == '?')
            {
                sf::Sprite questionBlock(questionBlockTexture);
                questionBlock.setPosition(x, y);
                questionBlocks.push_back(questionBlock);
            }
            else if (tile == ' ')
            {
                gaps.push_back(sf::FloatRect(x, y, TILE_SIZE, TILE_SIZE));
            }
            else if (tile == 'F')
            {
                if (!flagPlaced)
                {
                    flag.setTexture(flagTexture);
                    flag.setPosition(x, 530);
                    flag.setOrigin(flag.getGlobalBounds().width / 2, flag.getGlobalBounds().height / 2);
                    flag.setScale(0.3f, 0.3f);
                    flagPlaced = true;
                }
            }
        }
    }
}

/**
 * @brief Dessine les elements du niveau
 * @param window Fenetre de rendu
 * @details Cette methode dessine les elements du niveau
 */

void Background::draw(sf::RenderWindow &window)
{

    window.draw(sky);

    for (auto &tile : groundTiles)
    {
        window.draw(tile);
    }

    for (auto &brick : bricks)
    {
        window.draw(brick);
    }

    for (auto &block : questionBlocks)
    {
        window.draw(block);
    }

    for (auto &pipe : pipes)
    {
        window.draw(pipe);
    }

    for (auto &cloud : clouds)
    {
        window.draw(cloud);
    }
    if (flagPlaced)
    {
        window.draw(flag);
    }
}

/**
 * @brief Retourne les blocs de sol du niveau
 * @return Vector contenant les sprites des blocs de sol
 */
const sf::Sprite &Background::getFlag() const
{
    return flag;
}
std::vector<sf::FloatRect> Background::getGaps() const
{
    return gaps;
}
