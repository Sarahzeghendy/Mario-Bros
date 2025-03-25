#include "Headers/enemy.hpp"
#include "Headers/mouvement.hpp"
#include "Headers/player.hpp"
#include <iostream>


/**
 * @brief Constructeur de la classe FriendlyMushroom
 * @param x Position horizontale initiale du champignon
 * @param y Position verticale initiale du champignon
 * @details Initialise le champignon avec une texture, une vitesse et une échelle 
 */
FriendlyMushroom::FriendlyMushroom(float x, float y): 
    Enemy("images/champignon.png", x, y, 0, 0) // limites à 0 dans la classe parente Enemy
{
    sprite.setScale(0.1f, 0.1f); //réduction taille du sprite
    speed = 1.2f; 
}

/**
 * @brief Met à jour l'état du champignon
 * @details Gère les mouvements et les collisions uniquement si le champignon est en vie
 */
void FriendlyMushroom::update() 
{
    //ne fait rien -> champignon mort
    if (!alive) return; 
    
    // vérification champignon en train de tomber 
    if (isFalling) 
    {
        fall(); //applique gravité
        return;
    }
    
    //stockage position actuelle avant mouvement
    sf::Vector2f oldPosition = sprite.getPosition(); 

    //déplacement champignon
    if (movingRight) 
    {
        mouvement.moveRight();
    } 
    else 
    {
        mouvement.moveLeft();
    }

    // gestion collision + ajustage position
    handleCollisions(oldPosition);
}


/**
 * @brief Interaction avec le joueur
 * @param player Référence au joueur
 * @details Si le joueur est petit et touche le champignon, il grandit et le champignon disparaît
 */
void FriendlyMushroom::interactWithPlayer(Player& player) 
{
    //vérification collision
    if (alive && sprite.getGlobalBounds().intersects(player.getbounds())) 
    {
        //vérification taille du joueur 
        if (!player.isBig()) 
        {
            player.grow();
            alive = false; // disparition champignon
        }
    }
}

/**
 * @brief Réaction du champignon lorsqu'on saute dessus
 * @details Le champignon disparaît lorsqu'il est écrasé par le joueur
 */
void FriendlyMushroom::onJumpedOn()
{
    //vérification si champignon encore en vie
    if (alive) 
    {
        alive = false; //champignon meurt
    }
}

/**
 * @brief Inverse la direction du déplacement du champignon
 * @details Change l'orientation du mouvement lorsqu'il heurte un obstacle
 */
void FriendlyMushroom::reverseDirection() 
{
    movingRight = !movingRight; 
}