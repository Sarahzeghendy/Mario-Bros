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
      velocityY(0.0f), gravity(0.002f), jumpStrength(-0.8f), hangTime(20.0f), hangCounter(0.0f) {}

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
    if (!isJumping)
    {
        isJumping = true;
        velocityY = jumpStrength;
        hangCounter = hangTime;
    }

    if (velocityY < 0)
    {
        velocityY += gravity * 0.5f;
    }
    else
    {
        velocityY += gravity * 0.7f;
    }

    if (velocityY > 0.5f)
    {
        velocityY = 0.5f;
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
void Mouvement::applyGravity(const std::vector<sf::Sprite> &blocks, const std::vector<sf::Sprite> &pipes)
{
    
    if (velocityY < 0)
    {
        velocityY += gravity * 0.5f;
    }
    else
    {
        velocityY += gravity * 0.7f;
    }

    if (velocityY > 0.5f)
    {
        velocityY = 0.5f;
    }

    sprite.move(0, velocityY);

    bool onGround = false;

    // ✅ Vérifie la collision avec les blocs
    for (const auto &block : blocks)
    {
        if (sprite.getGlobalBounds().intersects(block.getGlobalBounds()))
        {
            if (velocityY > 0)
            {
                sprite.setPosition(sprite.getPosition().x, block.getPosition().y - sprite.getGlobalBounds().height);
                velocityY = 0;
                isJumping = false;
                hangCounter = 0;
                onGround = true;
            }
        }
    }

    for (const auto &pipe : pipes)
    {
        sf::FloatRect pipeBounds = pipe.getGlobalBounds();

        float pipeLeft = pipeBounds.left + 60.0f;   
        float pipeWidth = 35.4f;                    
        float topOfPipe = pipeBounds.top;
        float pipeHeight = pipeBounds.height;

        sf::FloatRect fixedPipeBounds(pipeLeft, topOfPipe, pipeWidth, pipeHeight);

        if (sprite.getGlobalBounds().intersects(fixedPipeBounds))
        {
            if (velocityY > 0)
            {
                sprite.setPosition(sprite.getPosition().x, topOfPipe - sprite.getGlobalBounds().height);
                velocityY = 0;
                isJumping = false;
                hangCounter = 0;
                onGround = true;
            }
        }
    }

    if (!onGround)
    {
        isJumping = true;
    }
}

/**
 * @brief Vérifie s'il y a un trou sous le joueur.
 * @param gaps Les trous avec lesquels vérifier la collision.
 */
void Mouvement::checkForGaps(const std::vector<sf::FloatRect> &gaps)
{
    for (const auto &gap : gaps)
    {
        if (sprite.getGlobalBounds().intersects(gap))
        {
            std::cout << "Le joueur est tombé dans un trou !" << std::endl;
            sprite.setPosition(100, 300);
            break;
        }
    }
}

/**
 * @brief Vérifie si quelque chose bloque le mouvement du joueur.
 * @param obstacles Les obstacles avec lesquels vérifier la collision.
 * @param pipes Les tuyaux avec lesquels vérifier la collision.
 * @return Vrai si le joueur peut bouger, faux sinon.
 */
std::pair<bool, bool> Mouvement::blockMovement(const std::vector<sf::Sprite> &obstacles, const std::vector<sf::Sprite> &pipes)
{
    bool canMoveRight = true;
    bool canMoveLeft = true;

    sf::FloatRect nextPositionRight = sprite.getGlobalBounds();
    sf::FloatRect nextPositionLeft = sprite.getGlobalBounds();
    nextPositionRight.left += speed;
    nextPositionLeft.left -= speed;

    for (const auto &obstacle : obstacles)
    {
        if (nextPositionRight.intersects(obstacle.getGlobalBounds()))
        {
            canMoveRight = false;
        }
        if (nextPositionLeft.intersects(obstacle.getGlobalBounds()))
        {
            canMoveLeft = false;
        }
    }

    for (const auto &pipe : pipes)
    {
        sf::FloatRect pipeBounds = pipe.getGlobalBounds();

        float pipeLeft = pipeBounds.left + 60.0f;
        float pipeWidth = 35.4f;
        float topOfPipe = pipeBounds.top;
        float pipeHeight = pipeBounds.height;

        sf::FloatRect fixedPipeBounds(pipeLeft, topOfPipe, pipeWidth, pipeHeight);

        if (nextPositionRight.intersects(fixedPipeBounds))
        {
            std::cout << "Collision à droite détectée ! Mouvement vers l'avant bloqué." << std::endl;
            canMoveRight = false;
            canMoveLeft = true; 
        }

        if (nextPositionLeft.intersects(fixedPipeBounds))
        {
            std::cout << "Collision à gauche détectée ! Mouvement vers l'arrière bloqué." << std::endl;
            canMoveLeft = false;
            canMoveRight = true;
        }
    }

    return {canMoveRight, canMoveLeft};
}
