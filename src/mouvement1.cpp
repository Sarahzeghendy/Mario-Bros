#include "Headers/mouvement1.hpp"
#include <iostream>

Mouvement::Mouvement(sf::Sprite &spr, float spd)
    : sprite(spr), speed(spd), isJumping(false),
      velocityY(0.0f), gravity(0.004f), jumpStrength(-0.9f), hangTime(40.0f), hangCounter(0.0f) {}


void Mouvement::moveRight()
{
    sprite.move(speed, 0);
}


void Mouvement::moveLeft()
{
    sprite.move(-speed, 0);
}


void Mouvement::jump()
{
    if (!isJumping) {
        isJumping = true;
        velocityY = jumpStrength;
        hangCounter = hangTime;
    }
    else if (hangCounter > 0) {
        // Si le joueur maintient le bouton de saut, il reste un peu plus en l'air
        velocityY = jumpStrength * 0.75f;
        hangCounter--;
    }
}


void Mouvement::applyGravity(const std::vector<sf::Sprite>& blocks, const std::vector<sf::Sprite>& pipes) {
    velocityY += gravity;
    sprite.move(0, velocityY);

    bool onGround = false;

    for (const auto& block : blocks) {
        if (sprite.getGlobalBounds().intersects(block.getGlobalBounds())) {
            if (velocityY > 0) {
                sprite.setPosition(sprite.getPosition().x, block.getPosition().y - sprite.getGlobalBounds().height);
                velocityY = 0;
                isJumping = false;
                hangCounter = 0;
                onGround = true;
            }
        }
    }

    for (const auto& pipe : pipes) {
        if (sprite.getGlobalBounds().intersects(pipe.getGlobalBounds())) {
            if (velocityY > 0) {
                sprite.setPosition(sprite.getPosition().x, pipe.getPosition().y - sprite.getGlobalBounds().height);
                velocityY = 0;
                isJumping = false;
                hangCounter = 0;
                onGround = true;
            }
        }
    }

    if (!onGround) {
        isJumping = true;
    }
}

void Mouvement::checkForGaps(const std::vector<sf::FloatRect>& gaps) {
    for (const auto& gap : gaps) {
        if (sprite.getGlobalBounds().intersects(gap)) {
            std::cout << "Le joueur est tombé dans un trou !" << std::endl;
            sprite.setPosition(100, 300); // ➡️ Mario est réinitialisé
            break;
        }
    }
}

bool Mouvement::blockMovement(const std::vector<sf::Sprite>& obstacles) {
    for (const auto& obstacle : obstacles) {
        if (sprite.getGlobalBounds().intersects(obstacle.getGlobalBounds())) {
            return false;
        }
    }
    return true;
}
