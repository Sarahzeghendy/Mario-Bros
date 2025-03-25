#include "Headers/enemy.hpp"
#include <iostream>



/**
 * @brief Constructeur de la classe Goomba
 * @param x Position horizontale de départ du Goomba
 * @param y Position verticale de départ du Goomba
 * @details Initialise le Goomba avec une position et une image de base, 
 *          en s'assurant que sa direction de déplacement initiale est vers la droite
 */
Goomba::Goomba(float x, float y): 
    Enemy("images/goomba.png", x, y, 0, 0) // limites à 0 dans la classe parente Enemy
{
    //Goomba se déplace en 1er vers la droite
    movingRight = true; 
    speed = 1.0f; 
}

/**
 * @brief Met à jour l'état du goomba
 * @details Gère les mouvements et les collisions uniquement si le champignon est en vie
 */
void Goomba::update() 
{
    //ne fait rien -> goomba mort
    if (!alive) return;

    // vérification goomba en train de tomber 
    if (isFalling)
    {
        fall();//applique gravité
        return;
    }

    //stockage position actuelle avant mouvement
    sf::Vector2f oldPosition = sprite.getPosition();

    //déplacement goomba
    if (movingRight) 
    {
        mouvement.moveRight();

        //vérification si goomba déjà orienté à droite
        if (sprite.getScale().x > 0) 
        {
            sprite.setScale(-0.1f, 0.1f); 
        }
    } 
    else 
    {
        mouvement.moveLeft();

        //vérification si goomba déjà orienté à gauche
        if (sprite.getScale().x < 0) 
        {
            sprite.setScale(0.1f, 0.1f);
        }
    }

    // gestion collision + ajustage position
    handleCollisionsGoomba(oldPosition);
}

/**
 * @brief Interaction avec le joueur
 * @param player Référence au joueur
 * @details Si le joueur entre en contact avec le Goomba, les effets sont appliqués selon 
 *          l'état du joueur (s'il est grand, petit, ou a un pouvoir spécial)
 */
void Goomba::interactWithPlayer(Player& player) 
{
    //vérification collision
    if (sprite.getGlobalBounds().intersects(player.getbounds())) 
    {
        //vérification si joueur a le pouvoir du feu
        if (player.hasFirePowerActive()) 
        {
            //joueur perd son pouvoir
            player.shrink();
        }
        //vérification taille du joueur 
        else if (player.isBig()) 
        {
            //joueur grand devient petit
            player.shrink();
        } 
        else 
        {
            //joueur petit -> meurt
            player.die();
        }
    }
}

/**
 * @brief Réaction du goomba lorsqu'on saute dessus
 * @details goomba disparaît lorsqu'il est écrasé par le joueur
 */
void Goomba::onJumpedOn() 
{
    //goomba meurt
    alive = false;

    //écrasement plus plat que les autres ennemis
    sprite.setScale(0.1f, 0.05f);  
}

/**
 * @brief Gère l'interaction de Goomba lorsqu'il est touché par une boule de feu
 * @details Lorsque Goomba est touché par une boule de feu, il meurt et se retourne
 */
void Goomba::onFireballHit() 
{
    //goomba meurt
    alive = false;

    //goomba se retourne pour simuler sa chute
    sprite.setScale(0.1f, -0.1f);  // Flip upside down
}

/**
 * @brief Inverse la direction du déplacement du goomba
 * @details Change l'orientation du mouvement lorsqu'il heurte un obstacle
 */
void Goomba::reverseDirection() 
{
    movingRight = !movingRight;
}

/**
 * @brief Gère les collisions du Goomba avec les tuyaux et blocs
 * @param oldPosition Position du Goomba avant le mouvement, utilisée pour le restaurer en cas de collision
 * @details Vérifie les collisions avec les tuyaux et blocs et ajuste la position et la direction 
 *          du Goomba en cas de collision
 */
void Goomba::handleCollisionsGoomba(const sf::Vector2f& oldPosition) 
{
    // définition boîtes englobantes plus larges pour détection des collisions à droite et à gauche
    sf::FloatRect globalBounds = sprite.getGlobalBounds();
    sf::FloatRect rightBounds(globalBounds.left + globalBounds.width - 10, globalBounds.top + 5, 10, globalBounds.height - 10); // Right edge
    sf::FloatRect leftBounds(globalBounds.left, globalBounds.top + 5, 10, globalBounds.height - 10); // Left edge

    // vérification des collisions avec tuyaux
    for (const auto& pipe : currentPipes) 
    {
        sf::FloatRect pipeBounds = pipe.getGlobalBounds();
        float pipeLeft = pipeBounds.left + 60.0f;
        float pipeWidth = 35.4f;
        sf::FloatRect fixedPipeBounds(pipeLeft, pipeBounds.top, pipeWidth, pipeBounds.height);

        //si goomba se déplace vers droite et touche tuyau -> inversion direction
        if (movingRight && rightBounds.intersects(fixedPipeBounds)) 
        {
            sprite.setPosition(oldPosition.x - 30.0f, oldPosition.y);//restaurer position
            reverseDirection();
            return; 
        }
         //si goomba se déplace vers gauche et touche tuyau -> inversion direction
        else if (!movingRight && leftBounds.intersects(fixedPipeBounds)) 
        {
            sprite.setPosition(oldPosition.x + 30.0f, oldPosition.y);//restaurer position
            reverseDirection();
            return; 
        }
    }

    //vérification collisions avec blocs
    for (const auto& block : currentBlocks) 
    {
        //si goomba se déplace vers droite et touche bloc -> inversion direction
        if (movingRight && rightBounds.intersects(block.getGlobalBounds())) 
        {
            sprite.setPosition(oldPosition.x - 25.0f, oldPosition.y);//restaurer position
            reverseDirection();
            return; 
        }
        //si goomba se déplace vers gauche et touche blocc -> inversion direction
        else if (!movingRight && leftBounds.intersects(block.getGlobalBounds())) 
        {
            sprite.setPosition(oldPosition.x + 25.0f, oldPosition.y);//restaurer position
            reverseDirection();
            return;  
        }
    }
}
