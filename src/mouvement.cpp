#include "Headers/mouvement.hpp"
#include <iostream>

/**
 * @brief Constructeur de la classe Mouvement
 * @param spr Reference au sprite du joueur
 * @param spd Vitesse horizontale du joueur
 * @details Ce constructeur initialise les attributs du joueur
 */
Mouvement::Mouvement(sf::Sprite &spr, float spd)
    : sprite(spr), speed(spd), isJumping(false),
      velocityY(0.0f), gravity(0.03f), jumpStrength(-4.5f), hangTime(10.0f), hangCounter(0.0f) {}

/**
 * @brief Deplace le joueur vers la droite
 * @details Cette methode deplace le joueur vers la droite en fonction de sa vitesse.
 * Si le joueur est en train de sauter, sa vitesse est reduite.
 */
void Mouvement::moveRight()
{
    float effectiveSpeed = isJumping ? speed * 0.7f : speed; // Reduire la vitesse en saut
    sprite.move(effectiveSpeed, 0);
}

/**
 * @brief Deplace le joueur vers la gauche
 * @details Cette methode deplace le joueur vers la gauche en fonction de sa vitesse.
 * Si le joueur est en train de sauter, sa vitesse est reduite.
 */
void Mouvement::moveLeft()
{
    float effectiveSpeed = isJumping ? speed * 0.7f : speed; // Reduire la vitesse en saut
    sprite.move(-effectiveSpeed, 0);
}

/**
 * @brief Saut du joueur
 * @details Cette methode permet au joueur de sauter.
 * Si le joueur est deja en train de sauter, il ne peut pas sauter a nouveau.
 * La vitesse verticale est ajustee pour simuler une montee freinee et une descente rapide.
 */
void Mouvement::jump()
{
    if (!isJumping)
    {
        isJumping = true;
        velocityY = jumpStrength; // Initialiser la force du saut
        hangCounter = hangTime;  // Temps de suspension en l'air
    }

    if (velocityY < 0)
    {
        velocityY += gravity * 1.0f; // Monter freinee
    }
    else
    {
        velocityY += gravity * 2.5f; // descente rapide
    }

    // Limiter la vitesse de descente
    if (velocityY > 2.5f)
    {
        velocityY = 2.5f;
    }
}

/**
 * @brief Applique la gravite et gere le saut en l'air
 * @param blocks Les blocs avec lesquels verifier la collision
 * @param pipes Les tuyaux avec lesquels verifier la collision
 * @param questionBlocks Les blocs de question avec lesquels verifier la collision
 * @details Cette methode applique la gravite au joueur et gere les collisions avec le sol et les obstacles.
 */
void Mouvement::applyGravity(const std::vector<sf::Sprite> &blocks, const std::vector<sf::Sprite> &pipes, const std::vector<sf::Sprite> &questionBlocks)
{
    // Fusionner les blocs et les blocs de question
    std::vector<sf::Sprite> allBlocks = blocks;
    allBlocks.insert(allBlocks.end(), questionBlocks.begin(), questionBlocks.end());

    // Ajuster la vitesse verticale en fonction de la gravite
    if (velocityY < 0)
    {
        velocityY += gravity * 1.0f; // Monter freinee
    }
    else
    {
        velocityY += gravity * 2.5f; // Descente rapide
    }

    // Limiter la vitesse de descente
    if (velocityY > 2.5f)
    {
        velocityY = 2.5f;
    }

    // Appliquer le mouvement vertical
    sprite.move(0, velocityY);

    bool onGround = false;

    // Verifier les collisions avec les blocs
    for (const auto &block : allBlocks)
    {
        if (sprite.getGlobalBounds().intersects(block.getGlobalBounds()))
        {
            if (velocityY > 0) // Collision par le bas
            {
                sprite.setPosition(sprite.getPosition().x, block.getPosition().y - sprite.getGlobalBounds().height);
                velocityY = 0;
                isJumping = false;
                hangCounter = 0;
                onGround = true;
            }
            else if (velocityY < 0) // Collision par le haut
            {
                sprite.setPosition(sprite.getPosition().x, block.getPosition().y + block.getGlobalBounds().height);
                velocityY = 0;
            }
        }
    }

    // Verifier les collisions avec les tuyaux (ajustement des dimensions pour une meilleure detection)
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
            if (velocityY > 0) // Collision par le bas
            {
                sprite.setPosition(sprite.getPosition().x, topOfPipe - sprite.getGlobalBounds().height);
                velocityY = 0;
                isJumping = false;
                hangCounter = 0;
                onGround = true;
            }
        }
    }

    // Si le joueur n'est pas sur le sol, il est en saut
    if (!onGround)
    {
        isJumping = true;
    }
}

/**
 * @brief Verifie s'il y a un trou sous le joueur
 * @param gaps Les trous avec lesquels verifier la collision
 * @details Cette methode verifie si le joueur tombe dans un trou et le repositionne si necessaire.
 */
void Mouvement::checkForGaps(const std::vector<sf::FloatRect> &gaps)
{
    for (const auto &gap : gaps)
    {
        if (sprite.getGlobalBounds().intersects(gap))
        {
            sprite.setPosition(100, 300); // repositionne le joueur en cas de chute
            break;
        }
    }
}

/**
 * @brief Verifie si quelque chose bloque le mouvement du joueur
 * @param obstacles Les obstacles avec lesquels verifier la collision
 * @param pipes Les tuyaux avec lesquels verifier la collision
 * @param questionBlocks Les blocs de question avec lesquels verifier la collision
 * @details Cette methode verifie si le joueur peut se deplacer a gauche ou a droite.
 * @return Une paire de booleens indiquant si le joueur peut bouger a droite et a gauche.
 */
std::pair<bool, bool> Mouvement::blockMovement(const std::vector<sf::Sprite> &obstacles, const std::vector<sf::Sprite> &pipes, const std::vector<sf::Sprite> &questionBlocks)
{
    // Fusionner les obstacles et les blocs de question
    std::vector<sf::Sprite> allObstacles = obstacles;
    allObstacles.insert(allObstacles.end(), questionBlocks.begin(), questionBlocks.end());

    bool canMoveRight = true;
    bool canMoveLeft = true;

    // Calculer les positions futures du joueur
    sf::FloatRect nextPositionRight = sprite.getGlobalBounds();
    sf::FloatRect nextPositionLeft = sprite.getGlobalBounds();
    nextPositionRight.left += speed; // Position future a droite
    nextPositionLeft.left -= speed;  // Position future a gauche

    // Verifier les collisions avec les obstacles
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

    // Verifier les collisions avec les tuyaux (ajustement des dimensions pour une meilleure detection)
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
            canMoveLeft = true; // Autorise le mouvement a gauche
        }

        if (nextPositionLeft.intersects(fixedPipeBounds))
        {
            canMoveLeft = false;
            canMoveRight = true; // Autorise le mouvement a droite
        }
    }

    return {canMoveRight, canMoveLeft};
}
