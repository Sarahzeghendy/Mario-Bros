#include "Headers/enemy.hpp"
#include "Headers/mouvement.hpp"
#include "Headers/player.hpp"
#include "Headers/friendlymushroom.hpp"
#include <iostream>


FriendlyMushroom::FriendlyMushroom(const std::string& imagePath, float x, float y, float leftLim, float rightLim)
    : Enemy(imagePath, x, y, leftLim, rightLim) 
{

    if (!texture.loadFromFile("images/champignon.png")) 
    {
        std::cerr << "Erreur: Impossible de charger l'image du champignon gentil\n";
    }
    sprite.setTexture(texture);
    sprite.setScale(0.1f, 0.1f);
}

void FriendlyMushroom::interactWithPlayer(Player& player) 
{
    if (sprite.getGlobalBounds().intersects(player.getbounds())) 
    {
        if (!player.isBig()) 
        {
            player.grow();
            std::cout << "Le joueur devient grand grâce au champignon gentil!" << std::endl;
        }
    }
}
