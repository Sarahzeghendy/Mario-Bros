#include "Headers/coin.hpp"
#include <iostream>

/**
 * @brief Constructeur de la classe Coin
 * @param x Position horizontale de la piece
 * @param y Position verticale de la piece
 * @param texturePath Chemin de limage de la piece
 */
Coin::Coin(float x, float y) : collected(false)
{
    if (!texture.loadFromFile("images/coins.png"))
    {
        std::cerr << "Erreur : Impossible de charger coin.png" << std::endl;
    }
    else
    {
        std::cout << "Pièce chargée" << std::endl;
    }
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
    sprite.setScale(0.9f, 0.9f);
}

/**
 * @brief Dessine la piece
 */
void Coin::draw(sf::RenderWindow &window)
{
    if (!collected)
    {
        window.draw(sprite);
    }
}

/**
 * @brief Renvoie les limites de la piece
 * @return Les limites de la piece
 */
sf::FloatRect Coin::getBounds() const
{
    return sprite.getGlobalBounds();
}

/**
 * @brief Indique si la piece a ete collectee
 * @return true si la piece a ete collectee false sinon
 */
bool Coin::isCollected() const
{
    return collected;
}

/**
 * @brief Collecte la piece
 * @details Cette methode est appelee lorsque le joueur entre en collision avec la piece
 * La piece est alors marquee comme collectee
 */
void Coin::collect()
{
    collected = true;
}
