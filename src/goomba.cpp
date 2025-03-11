#include "Headers/enemy.hpp"
#include <iostream>

Goomba::Goomba(float x, float y, float leftLim, float rightLim)
    : Enemy("images/goomba.png", x, y, leftLim, rightLim) 
{
}

void Goomba::update() 
{
    if (movingRight)
    {
        mouvement.moveRight();
        if (sprite.getPosition().x >= rightLimit) {
            movingRight = false;
        }
    }
    else
    {
        mouvement.moveLeft();
        if (sprite.getPosition().x <= leftLimit) {
            movingRight = true;
        }
    }
}

void Goomba::interactWithPlayer(Player& player) 
{
    if (sprite.getGlobalBounds().intersects(player.getbounds())) 
    {
        if (!player.isBig()) 
        {
            std::cout << "Le joueur est tué par le Goomba!" << std::endl;
            player.die();
        } 
        else 
        {
            player.shrink();
            std::cout << "Le joueur devient petit après avoir touché le Goomba!" << std::endl;
        }
    }
}

void Goomba::onJumpedOn() 
{
    alive = false;
    sprite.setScale(0.1f, 0.05f);  // Écrasé plus plat que les autres ennemis
    std::cout << "Goomba écrasé !" << std::endl;
}
