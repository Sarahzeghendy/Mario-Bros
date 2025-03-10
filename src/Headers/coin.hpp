#ifndef COIN_HPP
#define COIN_HPP

#include <SFML/Graphics.hpp>

/**
 * @class Coin
 * @brief Gère les pièces collectibles.
 */
class Coin {
private:
    /**
     * Attributes
     * @brief Texture de la pièce.
     * @brief Sprite de la pièce.
     * @brief collected : indique si la pièce a été ramassée.
     */
    sf::Texture texture;
    sf::Sprite sprite;
    bool collected; 

public:
    /**
     * @brief Constructeur de la classe Coin.
     * @param x Position horizontale de la pièce.
     * @param y Position verticale de la pièce.
     * @param texturePath Chemin de l'image de la pièce.
     */
    Coin(float x, float y);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const; 
    bool isCollected() const;
    void collect();
};

#endif
