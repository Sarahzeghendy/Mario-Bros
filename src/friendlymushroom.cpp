#include "Headers/enemy.hpp"
#include "Headers/mouvement.hpp"
#include "Headers/player.hpp"
#include "Headers/friendlymushroom.hpp"
#include <iostream>


FriendlyMushroom::FriendlyMushroom(float x, float y, float leftLim, float rightLim)
    : Enemy("images/champignon.png", x, y, leftLim, rightLim) 
{
    sprite.setScale(0.1f, 0.1f);
}

void FriendlyMushroom::update() 
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

void FriendlyMushroom::interactWithPlayer(Player& player) 
{
    if (alive && sprite.getGlobalBounds().intersects(player.getbounds())) 
    {
        if (!player.isBig()) 
        {
            player.grow();
            std::cout << "Le joueur devient grand grâce au champignon gentil!" << std::endl;
            alive = false; // Disparait après avoir été collecté
        }
    }
}


void FriendlyMushroom::onJumpedOn()
{
    
    if (alive) {
        alive = false;
        std::cout << "Champignon gentil collecté en sautant dessus!" << std::endl;
    }
}

