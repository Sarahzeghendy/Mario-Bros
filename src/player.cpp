#include "Headers/player.hpp"
#include "Headers/enemy.hpp"
#include <iostream>

/**
 * @brief Constructeur de la classe Player.
 * @param texturePath Chemin vers le fichier de texture du joueur.
 * @param x Position horizontale initiale du joueur.
 * @param y Position verticale initiale du joueur.
 * @param speed Vitesse horizontale du joueur.
 * @param right Touche pour déplacer le joueur vers la droite.
 * @param left Touche pour déplacer le joueur vers la gauche.
 * @param jump Touche pour faire sauter le joueur.
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

/**
 * @brief Charge la texture du joueur.
 */
bool Player::loadTexture() {
    return true;
}

/**
 * @brief Met à jour le joueur.
 * @param blocks Les blocs avec lesquels vérifier la collision.
 * @param pipes Les tuyaux avec lesquels vérifier la collision.
 */
void Player::update(const std::vector<sf::Sprite>& blocks, const std::vector<sf::Sprite>& pipes) {
    if (isDead) return;

    auto [canMoveRight, canMoveLeft] = mouvement.blockMovement(blocks, pipes);

    if (sf::Keyboard::isKeyPressed(rightKey) && canMoveRight) {
        mouvement.moveRight();
    }

    if (sf::Keyboard::isKeyPressed(leftKey) && canMoveLeft) {
        mouvement.moveLeft();
    }

    if (sf::Keyboard::isKeyPressed(jumpKey)) {
        mouvement.jump();
    }

    applyGravity(blocks, pipes);
}

/**
 * @brief Fait sauter le joueur.
 */
void Player::jump() {
    if (sf::Keyboard::isKeyPressed(jumpKey)) {
        mouvement.jump();
    }
}

/**
 * @brief Applique la gravité au joueur.
 * @param blocks Les blocs avec lesquels vérifier la collision.
 * @param pipes Les tuyaux avec lesquels vérifier la collision.
 */
void Player::applyGravity(const std::vector<sf::Sprite>& blocks, const std::vector<sf::Sprite>& pipes) {
    mouvement.applyGravity( blocks, pipes);
}

/**
 * @brief Vérifie s'il y a des trous sous le joueur.
 * @param gaps Les trous avec lesquels vérifier la collision.
 */
void Player::checkForGaps(const std::vector<sf::FloatRect> &gaps) {
    mouvement.checkForGaps(gaps);
}

/**
 * @brief Dessine le joueur.
 * @param window Fenêtre de rendu.
 */
void Player::draw(sf::RenderWindow& window) 
{
    if (!isDead) 
    {
    window.draw(sprite);
    }
}

/**
 * @brief Récupère une pièce.
 * @details Si le joueur a plus de 100 pièces, il gagne une vie.
 *         Le nombre de pièces est remis à zéro.
 *        Le nombre de vies est affiché.
 *       Le joueur gagne une vie.
 */
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

/**
 * @brief Perd une vie.
 * @details Si le joueur n'a plus de vies, affiche "Game Over !".
 */

void Player::loseLife() {
    lives--;
    std::cout << "Nombre de vies : " << lives << std::endl;

    if (lives == 0) {
        std::cout << "Game Over !" << std::endl;
    }
}

/**
 * @brief Gagne une vie.
 * @details Le nombre de vies est affiché.
 */
void Player::gainLife() {
    lives++;
    std::cout << "Nombre de vies : " << lives << std::endl;
}

/**
 * @brief Récupère le nombre de vies du joueur.
 */
int Player::getLives() const {
    return lives;
}

/**
 * @brief Tue le joueur.
 */
void Player::die() 
{
    isDead = true;  
    std::cout << "Le joueur est mort !" << std::endl;
}

/**
 * @brief Fait grandir le joueur.
 */
void Player::grow() 
{
    big = true;  
    sprite.setScale(0.2f, 0.2f); 
    sprite.setPosition(sprite.getPosition().x, 480.0f);
}

/**
 * @brief Fait rétrécir le joueur.
 */
void Player::shrink() 
{
    big = false;  
    sprite.setScale(0.1f, 0.1f); 
    sprite.setPosition(sprite.getPosition().x, 500.0f);
}

/**
 * @brief Vérifie si le joueur est grand.
 */
bool Player::isBig() const 
{
    return big;
}

/**
 * @brief Vérifie si le joueur a gagné.
 * @param flag Drapeau de fin de niveau.
 * @return true si le joueur a gagné, sinon false.
 */
bool Player::checkWin(const sf::Sprite& flag) {
    float playerX = sprite.getPosition().x + sprite.getGlobalBounds().width / 2;
    float playerY = sprite.getPosition().y + sprite.getGlobalBounds().height / 2;

    float flagX = flag.getPosition().x;
    float flagY = flag.getPosition().y;

    if (std::abs(playerX - flagX) < 10.0f && std::abs(playerY - flagY) < 15.0f) {
        std::cout << "Victoire !" << std::endl;
        return true;
    }

    return false;
}
