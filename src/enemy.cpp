#include "Headers/enemy.hpp"
#include "Headers/mouvement.hpp"
#include "Headers/player.hpp"
#include <iostream>


/**
 * @brief Constructeur de la classe Enemy
 * @param imagePath Chemin vers l'image du sprite de l'ennemi
 * @param x Position x initiale de l'ennemi
 * @param y Position y initiale de l'ennemi
 * @param leftLim Limite gauche de déplacement de l'ennemi
 * @param rightLim Limite droite de déplacement de l'ennemi
 * @details Initialise les attributs de l'ennemi, charge l'image et définit la position initiale
 */
Enemy::Enemy(const std::string &imagePath, float x, float y, float leftLim, float rightLim): 
    texture(),
    sprite(),
    alive(true),
    movingRight(false),
    mouvement(sprite, 0.5f), 
    leftLimit(leftLim),
    rightLimit(rightLim),
    speed(1.0f), 
    isFalling(false),
    fallSpeed(0.0f),
    GRAVITY(0.5f)
{
    if (!texture.loadFromFile(imagePath))
    {
        std::cerr << "Erreur: Impossible de charger l'image " << imagePath << "\n";
    }
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
    sprite.setScale(0.1f, 0.1f);
}

/**
 * @brief Affiche l'ennemi sur la fenêtre si l'ennemi est vivant
 * @param window La fenêtre de rendu dans laquelle l'ennemi est dessiné
 */
void Enemy::render(sf::RenderWindow &window)
{
    //si ennemi vivant
    if (alive)
    {
        window.draw(sprite);
    }
}


/**
 * @brief Retourne si l'ennemi est vivant
 * @return bool Vrai si l'ennemi est vivant, sinon faux
 */
bool Enemy::isAlive() const
{
    return alive;
}


/**
 * @brief Retourne les limites de l'ennemi sous forme de rectangle
 * @return sf::FloatRect Limites de l'ennemi
 */
sf::FloatRect Enemy::getBounds() const
{
    return sprite.getGlobalBounds();
}

/**
 * @brief Dessine l'ennemi dans la fenêtre
 * @param window La fenêtre de rendu dans laquelle l'ennemi est dessiné
 */
void Enemy::draw(sf::RenderWindow &window)
{
    window.draw(sprite);
}

/**
 * @brief Gère l'impact avec une boule de feu
 * @details L'ennemi est tué lorsque touché par une boule de feu
 */
void Enemy::onFireballHit()
{
    //ennemi meurt
    alive = false;
    std::cout << "Enemy defeated by fireball!" << std::endl;
}


/**
 * @brief Vérifie si l'ennemi est au-dessus d'un vide
 * @param gaps Liste des zones où l'ennemi peut tomber
 * @details Si l'ennemi est au-dessus d'un vide, il commence à tomber
 */
void Enemy::checkForGaps(const std::vector<sf::FloatRect> &gaps)
{
    //si ennemi mort ou en train de tomber -> ne fait rien
    if (!alive || isFalling)
        return;

    //zone des pieds de l'ennemi
    sf::FloatRect feetArea = sprite.getGlobalBounds();

    //placer zone en dessous des pieds
    feetArea.top += feetArea.height;

    feetArea.height = 5.0f;

    //vérifier si ennemi au dessus d'un trou
    for (const auto &gap : gaps)
    {
        //si zone pieds touche un trou
        if (feetArea.intersects(gap))
        {
            //ennemi commence à tomber
            isFalling = true;
            fallSpeed = 0.0f;
            std::cout << "Enemy fell into a gap!" << std::endl;
            return;
        }
    }
}

/**
 * @brief Applique la gravité pour faire tomber l'ennemi
 * @details Si l'ennemi est en chute, il descend avec une accélération due à la gravité
 */
void Enemy::fall()
{
    //si ennemi en train de tomber
    if (isFalling)
    {
        //application gravité
        fallSpeed += GRAVITY;

        //déplacement ennemi vers le bas
        sprite.move(0, fallSpeed);

        //si ennemi tombe hors de la fenêtre
        if (sprite.getPosition().y > 800)
        {
            //ennemi meurt
            alive = false;
            std::cout << "Ennemi tombé hors de la fenêtre" << std::endl;
        }
    }
}

/**
 * @brief Définir le niveau actuel pour l'ennemi (blocs, tuyaux et autres ennemis)
 * @param blocks Liste des blocs du niveau
 * @param pipes Liste des tuyaux du niveau
 * @param enemies Liste des autres ennemis dans le niveau
 */
void Enemy::setCurrentLevel(const std::vector<sf::Sprite> &blocks,
                            const std::vector<sf::Sprite> &pipes,
                            const std::vector<Enemy *> &enemies)
{
    //met à jour blocs, tuyaux, ennemis
    currentBlocks = blocks;
    currentPipes = pipes;
    currentEnemies = enemies;
}

/**
 * @brief Inverse la direction de déplacement de l'ennemi
 * @details Si l'ennemi se déplaçait à droite, il commencera à se déplacer à gauche et vice versa
 */
void Enemy::reverseDirection()
{
    movingRight = !movingRight;
}

/**
 * @brief Gère les collisions avec les blocs et les tuyaux
 * @param oldPosition Position de l'ennemi avant le mouvement
 */
void Enemy::handleCollisions(const sf::Vector2f &oldPosition)
{
    (void)oldPosition;

    sf::FloatRect globalBounds = sprite.getGlobalBounds();
    sf::FloatRect rightBounds(globalBounds.left + globalBounds.width - 10, globalBounds.top + 5, 10, globalBounds.height - 10); // Right edge
    sf::FloatRect leftBounds(globalBounds.left, globalBounds.top + 5, 10, globalBounds.height - 10);                            // Left edge

    //vérification collisions avec tuyaux 
    for (const auto &pipe : currentPipes)
    {
        sf::FloatRect pipeBounds = pipe.getGlobalBounds();
        float pipeLeft = pipeBounds.left + 60.0f;
        float pipeWidth = 35.4f;
        sf::FloatRect fixedPipeBounds(pipeLeft, pipeBounds.top, pipeWidth, pipeBounds.height);

        //si ennemi va à droite et touche tuyau
        if (movingRight && rightBounds.intersects(fixedPipeBounds))
        {
            sprite.setPosition(fixedPipeBounds.left - sprite.getGlobalBounds().width - 1, sprite.getPosition().y);
            reverseDirection();//inversion direction
            return;
        }
        //si ennemi va à gauche et touche tuyau
        else if (!movingRight && leftBounds.intersects(fixedPipeBounds))
        {
            sprite.setPosition(fixedPipeBounds.left + fixedPipeBounds.width + 1, sprite.getPosition().y);
            reverseDirection(); //inversion direction
            return;
        }
    }
    //vérification collisions avec blocs
    for (const auto &block : currentBlocks)
    {
        //si ennemi va à droite et touche bloc
        if (movingRight && rightBounds.intersects(block.getGlobalBounds()))
        {
            sprite.setPosition(block.getGlobalBounds().left - sprite.getGlobalBounds().width - 1, sprite.getPosition().y);
            reverseDirection();
            return;
        }
        //si ennemi va à gauche et touche bloc
        else if (!movingRight && leftBounds.intersects(block.getGlobalBounds()))
        {
            sprite.setPosition(block.getGlobalBounds().left + block.getGlobalBounds().width + 1, sprite.getPosition().y);
            reverseDirection();
            return;
        }
    }
}

/**
 * @brief Gère l'interaction avec le joueur
 * @param player Le joueur avec lequel l'ennemi interagit
 * @details Vérifie si l'ennemi touche le joueur et applique les effets appropriés en fonction de l'état du joueur
 */
void Enemy::interactWithPlayer(Player &player)
{
    //si joueur mort ou ennemi mort -> ne fait rien
    if (player.getIsDead() || !isAlive())
        return; 

    // si joueur invincible -> on ne fait rien 
    if (player.isInvincible())
        return;

    sf::FloatRect playerBounds = player.getbounds();
    sf::FloatRect enemyBounds = sprite.getGlobalBounds();

    //si joueur touche ennemi
    if (playerBounds.intersects(enemyBounds))
    {
        // si joueur a une grande vitesse (grâce à étoile)
        if (player.getSpeed() > player.getBaseSpeed())
        {
            //ennemi tué par saut joueur
            this->onJumpedOn();
            return;
        }

        // si joueur grand
        if (player.isBig())
        {
            //joueur devient petit
            player.shrink();
        }
        else
        {
            // si joueur petit -> meurt
            player.die();
        }
    }
}

