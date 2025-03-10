#include "Headers/player1.hpp"
#include <iostream>

Player::Player(const std::string& texturePath, float x, float y, float speed, 
               sf::Keyboard::Key right, sf::Keyboard::Key left, sf::Keyboard::Key jump) 
    : mouvement(sprite, speed), 
      rightKey(right), leftKey(left), jumpKey(jump),
      lives(3), coins(0), big(false), isDead(false) 
{
    if (!texture.loadFromFile(texturePath)) {
        throw std::runtime_error("Erreur : Impossible de charger " + texturePath);
    }

    sprite.setTexture(texture);
    sprite.setScale(0.1f, 0.1f);
    sprite.setPosition(x, y);
}


void Player::update(const std::vector<sf::Sprite>& blocks, const std::vector<sf::Sprite>& pipes) {
    if (isDead) return;


    bool canMove = mouvement.blockMovement(blocks);
    
    if (sf::Keyboard::isKeyPressed(rightKey) && canMove) {
        mouvement.moveRight();
    }
    if (sf::Keyboard::isKeyPressed(leftKey) && canMove) {
        mouvement.moveLeft();
    }


    if (sf::Keyboard::isKeyPressed(jumpKey)) {
        mouvement.jump();
    }

    applyGravity(blocks, pipes);
}


void Player::applyGravity(const std::vector<sf::Sprite>& blocks, const std::vector<sf::Sprite>& pipes) {
    mouvement.applyGravity(blocks, pipes);
}


void Player::jump() {
    mouvement.jump();
}

void Player::draw(sf::RenderWindow& window) {
    if (!isDead) {
        window.draw(sprite);
    }
}


void Player::getCoin() {
    coins++;
    std::cout << "Pièces : " << coins << std::endl;

    if (coins >= 100) {
        gainLife();   // Gagne une vie après 100 pièces
        coins = 0;
    }
}


void Player::loseLife() {
    lives--;
    std::cout << "Vies restantes : " << lives << std::endl;

    if (lives <= 0) {
        isDead = true;
        std::cout << "GAME OVER !" << std::endl;
    }
}

void Player::gainLife() {
    lives++;
    std::cout << "Vies restantes : " << lives << std::endl;
}


void Player::grow() {
    big = true;
    sprite.setScale(0.2f, 0.2f); // Devient plus grand
    sprite.setPosition(sprite.getPosition().x, sprite.getPosition().y - 20); // Légère correction verticale
}

void Player::shrink() {
    big = false;
    sprite.setScale(0.1f, 0.1f); // Devient plus petit
    sprite.setPosition(sprite.getPosition().x, sprite.getPosition().y + 20);
}


bool Player::isBig() const {
    return big;
}


