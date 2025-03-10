#include "Headers/coin.hpp"
#include <iostream>

/**
 * @brief Constructeur de la classe Coin.
 * @param x Position horizontale de la pièce.
 * @param y Position verticale de la pièce.
 * @param texturePath Chemin de l'image de la pièce.
 */
Coin::Coin(float x, float y) : collected(false) {
    if (!texture.loadFromFile("images/coins.png")) {
        std::cerr << "Erreur : Impossible de charger coin.png" << std::endl;
    } else {
        std::cout << "Pièce chargée" << std::endl;
    }
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
    sprite.setScale(0.9f, 0.9f);

    
}

/**
 * @brief Dessine la pièce.
 */
void Coin::draw(sf::RenderWindow& window) {
    if (!collected) {
        window.draw(sprite);
    }
}

/**
 * @brief Renvoie les limites de la pièce.
 * @return Les limites de la pièce.
 */
sf::FloatRect Coin::getBounds() const {
    return sprite.getGlobalBounds();
}


/**
 * @brief Indique si la pièce a été collectée.
 * @return true si la pièce a été collectée, false sinon.
 */
bool Coin::isCollected() const {
    return collected;
}

/**
 * @brief Collecte la pièce.
 */
void Coin::collect() {
    collected = true;
    std::cout << "Pièce collectée" << std::endl;
}
