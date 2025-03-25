#include "Headers/fireball.hpp"
#include <iostream>

/**
 * @brief Constructeur de la classe Fireball
 * @param x Position horizontale de la boule de feu
 * @param y Position verticale de la boule de feu
 * @param direction Direction de la boule de feu
 * @details Ce constructeur initialise la vitesse, la gravite, la position et l'etat de la boule de feu
 */
Fireball::Fireball(float x, float y, float direction)
    : speed(4.0f * direction), gravity(0.2f), velocityY(0.0f), active(true), initialX(x)
{
    if (!texture.loadFromFile("images/fireball.png"))
    {
        throw std::runtime_error("Erreur : Impossible de charger fireball.png");
    }

    sprite.setTexture(texture);
    sprite.setPosition(x, y);
    sprite.setScale(0.03f, 0.03f);
}

/**
 * @brief Met à jour la position de la boule de feu
 * @param blocks Vector contenant les sprites des blocs du niveau
 * @details Cette methode met a jour la position de la boule de feu en fonction de sa vitesse, de sa gravite et des collisions avec les blocs du niveau
 * Si la boule de feu sort de l'écran ou entre en collision avec un bloc, elle est marquée comme inactive
 */
void Fireball::update(const std::vector<sf::Sprite> &blocks)
{
    if (!active)
        return;

    sprite.rotate(10.0f);

    sprite.move(speed, velocityY);

    velocityY += gravity;

    if (velocityY > 5.0f)
    {
        velocityY = 5.0f;
    }

    for (const auto &block : blocks)
    {
        if (sprite.getGlobalBounds().intersects(block.getGlobalBounds()))
        {

            if (velocityY > 0)
            {
                velocityY = -4.0f;
            }
            else
            {
                active = false;
                return;
            }
        }
    }

    if (std::abs(sprite.getPosition().x - initialX) > 1000)
    {
        active = false;
    }

    if (sprite.getPosition().y > 800)
    {
        active = false;
    }
}

/**
 * @brief Dessine la boule de feu
 * @param window Fenetre de rendu
 * @details Cette methode dessine la boule de feu si elle est active
 */
void Fireball::draw(sf::RenderWindow &window)
{
    if (active)
    {
        window.draw(sprite);
    }
}
/**
 * @brief Renvoie les limites de la boule de feu
 * @return Les limites de la boule de feu
 */
sf::FloatRect Fireball::getBounds() const
{
    return sprite.getGlobalBounds();
}
/**
 * @brief Indique si la boule de feu est active
 * @return true si la boule de feu est active, false sinon
 */
bool Fireball::isActive() const
{
    return active;
}

/**
 * @brief Marque la boule de feu comme inactive
 */
void Fireball::destroy()
{
    active = false;
}
