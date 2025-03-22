#include "Headers/score.hpp"
#include <iostream>

Score::Score(const std::string& iconPath, const sf::Font& font, bool displayLives) : font(font), showLives(displayLives) {
    if (!iconTexture.loadFromFile(iconPath)) {
        std::cerr << "Error loading score icon: " << iconPath << std::endl;
    }
    
    icon.setTexture(iconTexture);
    icon.setScale(0.5f, 0.5f);  // Adjust scale as needed
    
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    
    if (showLives) {
        livesText.setFont(font);
        livesText.setCharacterSize(24);
        livesText.setFillColor(sf::Color::White);
    }
}

void Score::setPosition(float x, float y) {
    icon.setPosition(x, y);
    scoreText.setPosition(x + icon.getGlobalBounds().width + 10, y);
    
    if (showLives) {
        livesText.setPosition(x, y + 30);
    }
}

void Score::update(int score) {
    scoreText.setString(": " + std::to_string(score));
}

void Score::updateLives(int lives) {
    if (showLives) {
        livesText.setString("Lives: " + std::to_string(lives));
    }
}

void Score::draw(sf::RenderWindow& window) {
    window.draw(icon);
    window.draw(scoreText);
    
    if (showLives) {
        window.draw(livesText);
    }
}
