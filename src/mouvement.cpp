#include "Headers/mouvement.hpp"
#include <iostream>

/**
 * @brief Constructeur de la classe Mouvemen
 * @param spr Référence au sprite du joueur
 * @param spd Vitesse horizontale du joueur
 * @param ground Position du sol en Y
 * @param jumpStr Force du saut
 * @param hangT Temps de suspension en lair
 * @param grav Gravité appliquee au joueur
 * @details Ce constructeur initialise les attributs du joueur
 */

Mouvement::Mouvement(sf::Sprite &spr, float spd)
    : sprite(spr), speed(spd), isJumping(false),
      velocityY(0.0f), gravity(0.03f), jumpStrength(-4.5f), hangTime(10.0f), hangCounter(0.0f) {}

/**
 * @brief Deplace le joueur vers la droite
 * @details Cette methode deplace le joueur vers la droite en fonction de sa vitesse
 * Si le joueur est en train de sauter sa vitesse est reduite
 */
void Mouvement::moveRight()
{
    float effectiveSpeed = isJumping ? speed * 0.7f : speed;
    sprite.move(effectiveSpeed, 0);
}

/**
 * @brief Deplace le joueur vers la gauche
 * @details Cette methode deplace le joueur vers la gauche en fonction de sa vitesse
 * Si le joueur est en train de sauter sa vitesse est reduite
 */
void Mouvement::moveLeft()
{
    float effectiveSpeed = isJumping ? speed * 0.7f : speed;
    sprite.move(-effectiveSpeed, 0);
}

/**
 * @brief Saut du joueur
 * @details Cette methode permet au joueur de sauter
 * Si le joueur est deja en train de sauter il ne peut pas sauter a nouveau
 * Si le joueur est en train de sauter sa vitesse verticale est reduite
 * Si le joueur est en train de tomber sa vitesse verticale est augmentee
 * La vitesse verticale est limitee pour eviter une chute trop lente
 * Avec plusierus teste nous avons choisi les parametes qui nous paraissent les plus adapte
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
        velocityY += gravity * 1.0f; // Monter freinee
    }
    else
    {
        velocityY += gravity * 2.5f; // Descente rapide
    }

    // Eviter descente lente
    if (velocityY > 2.5f)
    {
        velocityY = 2.5f;
    }
}

/**
 * @brief Applique la gravite et gere le saut en lair
 * @param blocks Les blocs avec lesquels verifier la collision
 * @param pipes Les tuyaux avec lesquels verifier la collision
 * @param questionBlocks Les blocs de question avec lesquels verifier la collision
 * @details Cette methode applique la gravite au joueur et gere le saut en lair
 */
void Mouvement::applyGravity(const std::vector<sf::Sprite> &blocks, const std::vector<sf::Sprite> &pipes, const std::vector<sf::Sprite> &questionBlocks)
{
    std::vector<sf::Sprite> allBlocks = blocks;
    allBlocks.insert(allBlocks.end(), questionBlocks.begin(), questionBlocks.end());

    if (velocityY < 0)
    {
        velocityY += gravity * 1.0f;
    }
    else
    {
        velocityY += gravity * 2.5f;
    }

    if (velocityY > 2.5f)
    {
        velocityY = 2.5f;
    }

    sprite.move(0, velocityY);

    bool onGround = false;

    for (const auto &block : allBlocks)
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
            else if (velocityY < 0)
            {
                sprite.setPosition(sprite.getPosition().x, block.getPosition().y + block.getGlobalBounds().height);
                velocityY = 0;
            }
        }
    }

    // On a decaller la la detection des pipes pour une meilleur collision
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
 * @brief Verifie sil y a un trou sous le joueur
 * @param gaps Les trous avec lesquels verifier la collision
 * @details Cette methode verifie sil y a un trou sous le joueur
 */
void Mouvement::checkForGaps(const std::vector<sf::FloatRect> &gaps)
{
    for (const auto &gap : gaps)
    {
        if (sprite.getGlobalBounds().intersects(gap))
        {
            sprite.setPosition(100, 300);
            break;
        }
    }
}

/**
 * @brief Verifie si quelque chose bloque le mouvement du joueur
 * @param obstacles Les obstacles avec lesquels verifier la collision
 * @param pipes Les tuyaux avec lesquels verifier la collision
 * @param questionBlocks Les blocs de question avec lesquels verifier la collision
 * @details Cette methode verifie si quelque chose bloque le mouvement du joueur
 * @return Vrai si le joueur peut bouger faux sinon
 */
std::pair<bool, bool> Mouvement::blockMovement(const std::vector<sf::Sprite> &obstacles, const std::vector<sf::Sprite> &pipes, const std::vector<sf::Sprite> &questionBlocks)
{
    std::vector<sf::Sprite> allObstacles = obstacles;
    allObstacles.insert(allObstacles.end(), questionBlocks.begin(), questionBlocks.end());

    bool canMoveRight = true;
    bool canMoveLeft = true;

    sf::FloatRect nextPositionRight = sprite.getGlobalBounds();
    sf::FloatRect nextPositionLeft = sprite.getGlobalBounds();
    nextPositionRight.left += speed;
    nextPositionLeft.left -= speed;

    for (const auto &obstacle : allObstacles)
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
            canMoveRight = false;
            canMoveLeft = true;
        }

        if (nextPositionLeft.intersects(fixedPipeBounds))
        {
            canMoveLeft = false;
            canMoveRight = true;
        }
    }

    return {canMoveRight, canMoveLeft};
}
