#include "Headers/score.hpp"
#include <iostream>

Score::Score(const std::string& iconPath, const sf::Font& font, bool isLivesDisplay) 
    : font(font), isLivesDisplay(isLivesDisplay) {
    
    // Load character icon
    if (!iconTexture.loadFromFile(iconPath)) {
        std::cerr << "Error loading icon: " << iconPath << std::endl;
    }
    
    icon.setTexture(iconTexture);
    icon.setScale(0.5f, 0.5f);
    
    // For score display
    if (!isLivesDisplay) {
        scoreText.setFont(font);
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setString(": 0");
    }
    // For lives display
    else {
        // Load heart texture
        if (!heartTexture.loadFromFile("images/heart.png")) {
            std::cerr << "Error loading heart icon: images/heart.png" << std::endl;
        }
        
        // Create heart sprites
        for (int i = 0; i < MAX_HEARTS; i++) {
            sf::Sprite heartSprite;
            heartSprite.setTexture(heartTexture);
            heartSprite.setScale(0.1f, 0.1f);
            hearts.push_back(heartSprite);
        }
    }
}

void Score::setPosition(float x, float y) {
    icon.setPosition(x, y);
    
    if (!isLivesDisplay) {
        // Position score text
        scoreText.setPosition(x + icon.getGlobalBounds().width + 10, y);
    } else {
        // Position heart sprites
        float heartX = x + icon.getGlobalBounds().width + 10;
        for (int i = 0; i < MAX_HEARTS; i++) {
            hearts[i].setPosition(heartX + i * 30, y + 8);
        }
    }
}

void Score::update(int value) {
    if (!isLivesDisplay) {
        // Update score
        scoreText.setString(": " + std::to_string(value));
    } else {
        // Update hearts visibility based on lives count
        for (int i = 0; i < MAX_HEARTS; i++) {
            if (i < value) {
                hearts[i].setColor(sf::Color::White);
            } else {
                hearts[i].setColor(sf::Color(255, 255, 255, 0)); // Transparent
            }
        }
    }
}

void Score::draw(sf::RenderWindow& window) {
    // Always draw the character icon
    window.draw(icon);
    
    if (!isLivesDisplay) {
        // Draw score text
        window.draw(scoreText);
    } else {
        // Draw heart sprites
        for (auto& heart : hearts) {
            window.draw(heart);
        }
    }
}
