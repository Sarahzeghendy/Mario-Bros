#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class Score {
private:
    sf::Texture iconTexture;
    sf::Texture heartTexture;
    sf::Sprite icon;
    sf::Text scoreText;
    sf::Font font;
    bool isLivesDisplay;
    
    std::vector<sf::Sprite> hearts;
    const int MAX_HEARTS = 5;

public:
    Score(const std::string& iconPath, const sf::Font& font, bool isLivesDisplay = false);
    
    void setPosition(float x, float y);
    void update(int value); 
    void draw(sf::RenderWindow& window);
};
