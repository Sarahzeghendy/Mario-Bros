#include "Headers/enemy.hpp"
#include "Headers/mouvement.hpp"
#include "Headers/player.hpp"
#include <iostream>

Enemy::Enemy(const std::string& imagePath,float x, float y,  float leftLim, float rightLim) : alive(true), movingRight(true),leftLimit(leftLim), rightLimit(rightLim), mouvement(sprite, 0.3f) 
{
    if (!texture.loadFromFile("images/mushroom.png")) 
    {
        std::cerr << "Erreur: Impossible de charger l'image du champignon\n";
    }
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
    sprite.setScale(0.1f,0.1f); //remise à l'échelle
}

void Enemy::update() 
{

    // Déplacement vers la droite ou la gauche
    if (movingRight)
    {
        mouvement.moveRight();  // Déplacement vers la droite
        if (sprite.getPosition().x >= rightLimit) // Si on atteint la limite droite
        {
            movingRight = false;  // Inverser la direction
        }
    }
    else
    {
        mouvement.moveLeft();  // Déplacement vers la gauche
        if (sprite.getPosition().x <= leftLimit) // Si on atteint la limite gauche
        {
            movingRight = true;  // Inverser la direction
        }
    }
}

void Enemy::render(sf::RenderWindow& window) 
{
    if (alive) //dessine seulement si encore vivant
    {
        window.draw(sprite);
    }

}

void Enemy::onJumpedOn() 
{
    alive = false;  // L'ennemi est écrasé
    sprite.setScale(0.1f, 0.1f);  // Effet visuel : il est écrasé
    std::cout << "Champignon écrasé !" << std::endl; 
}

bool Enemy::isAlive() const 
{
    return alive;
}

sf::FloatRect Enemy::getBounds() const 
{
    return sprite.getGlobalBounds();
}

void Enemy::draw(sf::RenderWindow& window) 
{
    window.draw(sprite);  // Dessine le sprite de l'ennemi sur la fenêtre
}

void Enemy::interactWithPlayer(Player& player) 
{
    // Vérifie si le joueur touche l'ennemi (collision)
    if (sprite.getGlobalBounds().intersects(player.getbounds())) 
    {
        if (!player.isBig()) 
        {
            // Si le joueur est petit, il est tué
            std::cout << "Le joueur est tué par le champignon!" << std::endl;
            player.die();  // Appel à la méthode die() pour marquer la mort
        } 
        else 
        {
            // Si le joueur est grand, il devient petit
            player.shrink();
            std::cout << "Le joueur devient petit après avoir touché le champignon!" << std::endl;
        }
    }
}
