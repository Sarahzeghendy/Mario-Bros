#pragma once
#ifndef SCORE_HPP
#define SCORE_HPP

#include <SFML/Graphics.hpp>
#include <string>

class Score {
private:
    sf::Texture iconTexture;
    sf::Sprite icon;
    sf::Text scoreText;
    sf::Font font;

public:
    Score(const std::string& iconPath, const sf::Font& font);
    void setPosition(float x, float y);
    void update(int score);
    void draw(sf::RenderWindow& window);
};

#endif
