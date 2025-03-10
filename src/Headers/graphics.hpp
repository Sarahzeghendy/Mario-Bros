#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

class Graphics {
private:
    sf::RenderWindow& window;
    sf::Color backgroundColor;
    
    // Textures
    sf::Texture blockTexture;
    sf::Texture pipeTexture;

    // Sprites
    std::vector<sf::Sprite> groundBlocks;
    std::vector<sf::Sprite> floatingBlocks;
    sf::Sprite pipeSprite;

public:
    Graphics(sf::RenderWindow& win);
    bool loadTextures();
    void setupScene();
    void render();
    void moveScene(float dx);

};

#endif
