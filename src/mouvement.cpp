#include "Headers/mouvement.hpp"
#include <iostream>

/**
 * @brief Constructeur de la classe Mouvement.
 * @param spr Référence au sprite du joueur.
 * @param spd Vitesse horizontale du joueur.
 * @param ground Position du sol en Y.
 */

Mouvement::Mouvement(sf::Sprite &spr, float spd)
    : sprite(spr), speed(spd), isJumping(false),
      velocityY(0.0f), gravity(0.004f), jumpStrength(-1.2f), hangTime(40.0f), hangCounter(0.0f) {}

/**
 * @brief Déplace le joueur vers la droite.
 */
void Mouvement::moveRight()

{
    sprite.move(speed, 0);
}

/**
 * @brief Déplace le joueur vers la gauche.
 */
void Mouvement::moveLeft()
{
    sprite.move(-speed, 0);
}

/**
 * @brief Saut du joueur.
 */
void Mouvement::jump()
{
    if (!isJumping) {
        isJumping = true;
        velocityY = jumpStrength;
        hangCounter = hangTime;
    }
    else if (hangCounter > 0) {
        velocityY = jumpStrength * 0.75f;
        hangCounter--;
    }
    if (velocityY < 0) // Montée
    {
        velocityY += gravity * 0.1f;
    }
    else
    {
        velocityY += gravity * 0.6f; // Descente
    }
}

/**
 * @brief Vérifie si le joueur est en collision avec un bloc.
 * @param block Bloc avec lequel vérifier la collision.
 * @return Vrai si le joueur est en collision avec le bloc, faux sinon.
 */
bool Mouvement::checkCollision(const sf::Sprite &object)
{
    return sprite.getGlobalBounds().intersects(object.getGlobalBounds());
}

/**
 * @brief Applique la gravité et gère le saut en l'air.
 * @param blocks Les blocs avec lesquels vérifier la collision.
 * @param pipes Les tuyaux avec lesquels vérifier la collision.
 */
void Mouvement::applyGravity(const std::vector<sf::Sprite>& blocks, const std::vector<sf::Sprite>& pipes) {
    velocityY += gravity;
    sprite.move(0, velocityY);

    bool onGround = false;

    for (const auto& block : blocks) {
        if (sprite.getGlobalBounds().intersects(block.getGlobalBounds())) {
            if (velocityY > 0) {
                sprite.setPosition(sprite.getPosition().x, block.getPosition().y - sprite.getGlobalBounds().height);
                velocityY = 0;
                isJumping = false;
                hangCounter = 0;
                onGround = true;
            }
        }
    }

    for (const auto& pipe : pipes) {
        if (sprite.getGlobalBounds().intersects(pipe.getGlobalBounds())) {
            if (velocityY > 0) {
                sprite.setPosition(sprite.getPosition().x, pipe.getPosition().y - sprite.getGlobalBounds().height);
                velocityY = 0;
                isJumping = false;
                hangCounter = 0;
                onGround = true;
            }
        }
    }
    if (!onGround) {
        isJumping = true;
    }
}
/**
 * @brief 
 * 
 */
void Mouvement::checkForGaps(const std::vector<sf::FloatRect>& gaps) {
    for (const auto& gap : gaps) {
        if (sprite.getGlobalBounds().intersects(gap)) {
            std::cout << "Le joueur est tombé dans un trou !" << std::endl;
            sprite.setPosition(100, 300); 
            break;
        }
    }
}

/**
 * 
 */
bool Mouvement::blockMovement(const std::vector<sf::Sprite>& obstacles, const std::vector<sf::Sprite>& pipes) {
    for (const auto& obstacle : obstacles) {
        if (sprite.getGlobalBounds().intersects(obstacle.getGlobalBounds())) {
            return false; 
        }
    }

    for (const auto& pipe : pipes) {
        if (sprite.getGlobalBounds().intersects(pipe.getGlobalBounds())) {
            return false; 
        }
    }

    return true;
}
