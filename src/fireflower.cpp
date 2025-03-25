#include "Headers/fireflower.hpp"
#include <iostream>

/**
 * @brief Constructeur de la classe FireFlower
 * @param x Position horizontale de la fleur de feu
 * @param y Position verticale de la fleur de feu
 * @details Ce constructeur initialise la texture la position et letat de la fleur de feu
 */
FireFlower::FireFlower(float x, float y)
    : collected(false)
{
    if (!texture.loadFromFile("images/fireflower.png"))
    {
        throw std::runtime_error("Erreur : Impossible de charger fireflower.png");
    }

    sprite.setTexture(texture);
    sprite.setPosition(x, y);
    sprite.setScale(0.1f, 0.1f);
}

/**
 * @brief Dessine la fleur de feu
 */
void FireFlower::draw(sf::RenderWindow &window)
{
    if (!collected)
    {
        window.draw(sprite);
    }
}

/**
 * @brief Renvoie les limites de la fleur de feu
 * @return Les limites de la fleur de feu
 */
sf::FloatRect FireFlower::getBounds() const
{
    return sprite.getGlobalBounds();
}

/**
 * @brief Indique si la fleur de feu a ete collectee
 * @return true si la fleur de feu a ete collectee false sinon
 */
bool FireFlower::isCollected() const
{
    return collected;
}

/**
 * @brief Collecte la fleur de feu
 * @details Cette methode est appelee lorsque le joueur entre en collision avec la fleur de feu
 * La fleur de feu est alors marquee comme collectee
 */
void FireFlower::collect()
{
    collected = true;
    std::cout << "Mario a collecté une Fleur de Feu !" << std::endl;
}
