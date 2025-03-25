#include "Headers/etoile.hpp"
#include <iostream>


/**
 * @brief Constructeur de la classe Etoile
 * @param x Position horizontale de l'étoile
 * @param y Position verticale de l'étoile
 * @details Ce constructeur initialise la texture la position et l'état de l'étoile
 */
Etoile::Etoile(float x, float y)
    : collected(false)
{
    if (!texture.loadFromFile("images/etoile.png"))
    {
        throw std::runtime_error("Erreur : Impossible de charger fireflower.png");
    }

    sprite.setTexture(texture);
    sprite.setPosition(x, y);
    sprite.setScale(0.1f, 0.1f); 
}

/**
 * @brief Dessine l'étoile
 */
void Etoile::draw(sf::RenderWindow &window)
{
    if (!collected)
    {
        window.draw(sprite);
    }
}

/**
 * @brief Renvoie les limites de l'étoile
 * @return Les limites de l'étoile
 */
sf::FloatRect Etoile::getBounds() const
{
    return sprite.getGlobalBounds();
}

/**
 * @brief Indique si l'étoile a été collectée
 * @return true si l'étoile a été collectée false sinon
 */
bool Etoile::isCollected() const
{
    return collected;
}

/**
 * @brief Collecte l'étoile
 * @details Cette methode est appelée lorsque le joueur entre en collision avec l'étoile
 * L'étoile est alors marquée comme collectée
 */
void Etoile::collect()
{
    collected = true;
    std::cout << "Mario a collecté une étoile !" << std::endl;
}
