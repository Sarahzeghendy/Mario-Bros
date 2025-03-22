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
    sf::Text livesText;
    sf::Font font;
    bool showLives;

public:
    Score(const std::string& iconPath, const sf::Font& font, bool displayLives = false);
    void setPosition(float x, float y);
    void update(int score);
    void updateLives(int lives);
    void draw(sf::RenderWindow& window);
};

#endif
