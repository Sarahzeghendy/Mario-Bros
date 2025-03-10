#include "Headers/player.hpp"
#include "Headers/enemy.hpp"
#include <iostream>

/**
 * @brief Constructeur de la classe Player.
 * @param texturePath Chemin de l'image du joueur.
 * @param x Position horizontale du joueur.
 * @param y Position verticale du joueur.
 * @param speed Vitesse horizontale du joueur.
 * @param right Touche pour se déplacer à droite.
 * @param jump Touche pour sauter.
 * @param lives Nombre de vies du joueur. Le joueur commence avec 3 vies.
 * @param coins Nombre de pièces collectées par le joueur. Le joueur commence avec 0 pièces.
 */
Player::Player(const std::string& texturePath, float x, float y, float speed, sf::Keyboard::Key right,sf::Keyboard::Key left, sf::Keyboard::Key jump) 
    : mouvement(sprite, speed), rightKey(right), leftKey(left), jumpKey(jump), lives(3), coins(0), big(true), isDead(false), movingRight(false), movingLeft(false) 
{
   
    if (!texture.loadFromFile(texturePath)) 
    {
        std::cerr << "Erreur : Impossible de charger " << texturePath << std::endl;
    }
    sprite.setTexture(texture);
    sprite.setScale(0.15f, 0.15f);
    sprite.setPosition(x, y);
}


bool Player::loadTexture() {
    return true;
}

void Player::update(const std::vector<sf::Sprite>& blocks, const std::vector<sf::Sprite>& pipes) {
    if (isDead) return;

    // ➡️ Déplacement horizontal (si autorisé)
    bool canMove = mouvement.blockMovement(blocks, pipes);
    
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

void Player::jump() {
    if (sf::Keyboard::isKeyPressed(jumpKey)) {
        mouvement.jump();
    }
}

void Player::applyGravity(const std::vector<sf::Sprite>& blocks, const std::vector<sf::Sprite>& pipes) {
    mouvement.applyGravity( blocks, pipes);
}

void Player::checkForGaps(const std::vector<sf::FloatRect> &gaps) {
    mouvement.checkForGaps(gaps);
}

void Player::draw(sf::RenderWindow& window) 
{
    if (!isDead) 
    {
    window.draw(sprite);
    }
}

void Player::getCoins() 
{
    coins++;
    std::cout << "Nombre de pièces : " << coins << std::endl;

    if (coins > 100) 
    {
        std::cout << "Vous avez gagné une vie !" << std::endl;
        gainLife();
        std::cout << "Nombre de vies : " << lives << std::endl;
        coins = 0;
    }
}

void Player::loseLife() {
    lives--;
    std::cout << "Nombre de vies : " << lives << std::endl;

    if (lives == 0) {
        std::cout << "Game Over !" << std::endl;
    }
}

void Player::gainLife() {
    lives++;
    std::cout << "Nombre de vies : " << lives << std::endl;
}

int Player::getLives() const {
    return lives;
}

void Player::die() 
{
    isDead = true;  // Le joueur est mort
    std::cout << "Le joueur est mort !" << std::endl;
}

void Player::grow() 
{
    big = true;  // Le joueur devient grand
    sprite.setScale(0.2f, 0.2f); // Mettre à jour le sprite pour le rendre plus grand
    sprite.setPosition(sprite.getPosition().x, 480.0f);
}

void Player::shrink() 
{
    big = false;  // Le joueur devient petit
    sprite.setScale(0.1f, 0.1f); // Mettre à jour le sprite pour le rendre plus petit
    sprite.setPosition(sprite.getPosition().x, 500.0f);
}

bool Player::isBig() const 
{
    return big;
}

