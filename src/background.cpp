#include "Headers/background.hpp"
#include <fstream>
#include <iostream>

const float TILE_SIZE = 40.0f;

Background::Background()
{

    sky.setSize(sf::Vector2f(1280, 720));
    sky.setFillColor(sf::Color(135, 206, 250)); 

    if (!groundBlock.loadFromFile("images/block.png"))
    {
        throw std::runtime_error("Erreur : Impossible de charger block.png");
    }
    if (!questionBlockTexture.loadFromFile("images/coin.png"))
    {
        throw std::runtime_error("Erreur : Impossible de charger coin.png");
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

    loadMap("map.txt");

    for (int i = 0; i < 5; i++)
    {
        sf::Sprite cloud(cloudTexture);
        cloud.setPosition(i * 300, 50);
        cloud.setScale(0.2f, 0.2f);
        clouds.push_back(cloud);
    }
}

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
    float windowHeight = 720.0f;
    float verticalOffset = 50.0f;

    if (verticalOffset < 0)
        verticalOffset = 0;

    for (size_t row = 0; row < lines.size(); row++)
    {
        for (size_t col = 0; col < lines[row].size(); col++)
        {
            char tile = lines[row][col];
            float x = col * TILE_SIZE;
            float y = row * TILE_SIZE + verticalOffset;

            if (tile == '#')
            {
                sf::Sprite ground(groundBlock);
                ground.setPosition(x, y);
                ground.setScale(0.043f, 0.043f);
                groundTiles.push_back(ground);
            }
            else if (tile == 'P' || tile == 'p')
            {
                sf::Sprite pipe(pipeTexture);
                pipe.setPosition(100, 430);
                pipe.setScale(0.3f, 0.3f);

                pipes.push_back(pipe);
            }
            else if (tile == '?')
            {
                sf::Sprite questionBlock(questionBlockTexture);
                questionBlock.setPosition(x, y);
                questionBlock.setScale(0.043f, 0.043f);
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

const sf::Sprite& Background::getFlag() const {
    return flag;
}
