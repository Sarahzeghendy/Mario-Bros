#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include <SFML/Graphics.hpp>
#include <vector>

class Background
{
public:
    Background();
    void draw(sf::RenderWindow &window);
    std::vector<sf::Sprite> pipes;
    std::vector<sf::Sprite> groundTiles;
    const std::vector<sf::Sprite> &getGroundTiles() const
    {
        return groundTiles;
    }

    const std::vector<sf::Sprite> &getPipes() const
    {
        return pipes;
    }
    const sf::Sprite &getFlag() const;
    std::vector<sf::FloatRect> getGaps() const;

private:
    sf::RectangleShape sky;
    sf::Texture groundBlock;
    sf::Texture questionBlockTexture;
    sf::Texture pipeTexture;
    sf::Texture cloudTexture;

    std::vector<sf::Sprite> bricks;
    std::vector<sf::Sprite> questionBlocks;

    std::vector<sf::Sprite> clouds;

    std::vector<sf::FloatRect> gaps;
    sf::Texture flagTexture;
    sf::Sprite flag;
    bool flagPlaced = false;

    void loadMap(const std::string &filePath);
};

#endif
