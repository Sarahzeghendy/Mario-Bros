#include "Headers/enemy.hpp"
#include <iostream>

KoopaTroopa::KoopaTroopa(float x, float y, float leftLim, float rightLim)
    : Enemy("images/koopa.png", x, y, leftLim, rightLim) 
{
    sprite.setScale(0.15f, 0.15f);
    if (!koopaShell.loadFromFile("images/koopa_closed.png")) {
        std::cerr << "Erreur: Impossible de charger koopa_closed.png\n";
    }
}

void KoopaTroopa::update() 
{
    if (movingRight)
    {
        mouvement.moveRight();
        if (sprite.getPosition().x >= rightLimit) {
            movingRight = false;
            sprite.setScale(-0.15f, 0.15f);  // On flip le sprite
        }
    }
    else
    {
        mouvement.moveLeft();
        if (sprite.getPosition().x <= leftLimit) {
            movingRight = true;
            sprite.setScale(0.15f, 0.15f);  // On remet le sprite à l'endroit
        }
    }
}

void KoopaTroopa::interactWithPlayer(Player& player) 
{
    if (sprite.getGlobalBounds().intersects(player.getbounds())) 
    {
        if (!player.isBig()) 
        {
            std::cout << "Le joueur est tué par le Koopa!" << std::endl;
            player.die();
        } 
        else 
        {
            player.shrink();
            std::cout << "Le joueur devient petit après avoir touché le Koopa!" << std::endl;
        }
    }
}

void KoopaTroopa::onJumpedOn() 
{
    sprite.setTexture(koopaShell);
    std::cout << "Koopa dans sa carapace!" << std::endl;
}
