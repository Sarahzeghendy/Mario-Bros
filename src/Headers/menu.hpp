#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Menu
{
public:
    Menu(float width, float height);

    void draw(sf::RenderWindow &window);

    void moveUp();

    void moveDown();

    int getSelectedIndex() const;

    void showRules();

private:
    sf::Font font;
    std::vector<sf::Text> menuOptions;
    int selectedIndex;
    sf::RectangleShape selector;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    std::vector<sf::Texture> frames;
    sf::Clock frameClock;
    int currentFrame = 0;
};

#endif
