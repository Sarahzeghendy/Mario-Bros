#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

/**
 * @class Menu
 * @brief Gère le menu du jeu.
 */
class Menu {
public:
    /**
     * @brief Constructeur de la classe Menu.
     * @param width Largeur de la fenêtre.
     * @param height Hauteur de la fenêtre.
     */
    Menu(float width, float height);

    /**
     * @brief Dessine le menu.
     * @param window Fenêtre de rendu.
     */
    void draw(sf::RenderWindow &window);

    /**
     * @brief Déplace le cadre de sélection vers le haut.
     */
    void moveUp();

    /**
     * @brief Déplace le cadre de sélection vers le bas.
     */
    void moveDown();

    /**
     * @brief Retourne l'indice de l'option sélectionnée.
     * @return Indice de l'option sélectionnée.
     */
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
