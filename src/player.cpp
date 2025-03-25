#include "Headers/player.hpp"
#include "Headers/enemy.hpp"
#include <iostream>

/**
 * @brief Constructeur de la classe Player
 * @param texturePath Chemin de limage du joueur
 * @param name Nom du joueur
 * @param x Position horizontale du joueur
 * @param y Position verticale du joueur
 * @param speed Vitesse du joueur
 * @param right Touche pour deplacer le joueur vers la droite
 * @param left Touche pour deplacer le joueur vers la gauche
 * @param jump Touche pour faire sauter le joueur
 * @details Ce constructeur initialise les attributs du joueur
 */

Player::Player(const std::string &texturePath, const std::string &name, float x, float y, float speed,
               sf::Keyboard::Key right, sf::Keyboard::Key left, sf::Keyboard::Key jump)
    : normalTexture(),
      fireTexture(),
      sprite(),
      characterName(name),
      mouvement(sprite, speed),
      rightKey(right),
      leftKey(left),
      jumpKey(jump),
      big(true),
      isDead(false),
      movingRight(false),
      movingLeft(false),
      hasFirePower(false),
      lives(3),
      coins(0),
      fireballCooldown(0),
      fireballs(),
      currentFrame(0),
      frameCounter(0),
      characterType(),
      hitTimer(0),
      baseSpeed(speed),
      currentSpeed(speed),
      score(0),
      initialX(x),
      initialY(y),
      deathX(x),
      deathY(y),
      lastLifeThreshold(0)
{

    if (!normalTexture.loadFromFile(texturePath))
    {
        std::cerr << "Erreur : Impossible de charger " << texturePath << std::endl;
    }

    std::string fireTexturePath;
    if (characterName == "Mario")
    {
        fireTexturePath = "images/mario_fire.png";
    }
    else if (characterName == "Luigi")
    {
        fireTexturePath = "images/luigi_fire.png";
    }
    else
    {

        fireTexturePath = "images/mario_fire.png";
    }

    if (!fireTexture.loadFromFile(fireTexturePath))
    {
        std::cerr << "Erreur : Impossible de charger " << fireTexturePath << std::endl;
        fireTexture = normalTexture;
    }
    else
    {
        std::cout << "Fire texture loaded successfully: " << fireTexturePath << std::endl;
    }

    sprite.setTexture(normalTexture);
    sprite.setScale(1.2f, 1.2f);
    sprite.setPosition(x, y);
}

/**
 * @brief Charge la texture du joueur
 */
bool Player::loadTexture()
{
    return true;
}

/**
 * @brief Met a jour les droit de mouvement du joueur
 * @param blocks Les blocs avec lesquels verifier la collision
 * @param pipes Les tuyaux avec lesquels verifier la collision
 * @param questionBlocks Les blocs de question avec lesquels verifier la collision
 * @details Cette méthode met  a jour les droits de mouvement du joueur
 */
void Player::update(const std::vector<sf::Sprite> &blocks, const std::vector<sf::Sprite> &pipes, const std::vector<sf::Sprite> &questionBlocks)
{

    updatePreviousPosition();

    if (isDead)
        return;

    if (hitTimer > 0)
    {
        hitTimer--;

        if (hitTimer % 10 < 5)
        {
            sprite.setColor(sf::Color(255, 255, 255, 128));
        }
        else
        {
            sprite.setColor(sf::Color(255, 255, 255, 255));
        }

        if (hitTimer == 0)
        {
            sprite.setColor(sf::Color(255, 255, 255, 255));
        }
    }

    if (fireballCooldown > 0)
    {
        fireballCooldown--;
    }

    movingRight = sf::Keyboard::isKeyPressed(rightKey);
    movingLeft = sf::Keyboard::isKeyPressed(leftKey);

    updateFireballs(blocks);

    auto [canMoveRight, canMoveLeft] = mouvement.blockMovement(blocks, pipes, questionBlocks);

    if (sf::Keyboard::isKeyPressed(rightKey) && canMoveRight)
    {
        mouvement.moveRight();
        movingRight = true;
        movingLeft = false;
    }

    if (sf::Keyboard::isKeyPressed(leftKey) && canMoveLeft)
    {
        mouvement.moveLeft();
        movingLeft = true;
        movingRight = false;
    }

    if (sf::Keyboard::isKeyPressed(jumpKey))
    {
        mouvement.jump();
    }

    if (isStarPowered)
    {
        starPowerFrames--;

        if (starPowerFrames <= 0)
        {

            currentSpeed = baseSpeed;
            mouvement.setSpeed(currentSpeed);
            isStarPowered = false;
        }
    }

    applyGravity(blocks, pipes, questionBlocks);

    animate();
}

/**
 * @brief Fait sauter le joueur
 */
void Player::jump()
{
    if (sf::Keyboard::isKeyPressed(jumpKey))
    {
        mouvement.jump();
    }
}

/**
 * @brief Applique la gravite au joueur
 * @param blocks Les blocs avec lesquels verifier la collision
 * @param pipes Les tuyaux avec lesquels verifier la collision
 * @param questionBlocks Les blocs de question avec lesquels verifier la collision
 * @details Cette methode applique la gravite au joueur
 */
void Player::applyGravity(const std::vector<sf::Sprite> &blocks, const std::vector<sf::Sprite> &pipes, const std::vector<sf::Sprite> &questionBlocks)
{
    mouvement.applyGravity(blocks, pipes, questionBlocks);
}

/**
 * @brief Verifie sil y a des trous sous le joueur
 * @param gaps Les trous avec lesquels verifier la collision.
 */
void Player::checkForGaps(const std::vector<sf::FloatRect> &gaps)
{
    mouvement.checkForGaps(gaps);
}

/**
 * @brief Dessine le joueure
 * @param window Fenetre de rendu
 */
void Player::draw(sf::RenderWindow &window)
{
    if (!isDead)
    {
        window.draw(sprite);
    }
}

/**
 * @brief Ajoute des points au score et verifie les seuils pour gagner des vies
 * @param points Nombre de points à ajouter
 * @details Cette methode ajoute des points au score du joueur et verifie si le joueur a atteint un seuil de 100 points pour gagner une vie
 */
void Player::addScore(int points)
{
    int oldScore = score;
    score += points;

    int oldThreshold = oldScore / 100;
    int newThreshold = score / 100;

    if (newThreshold > oldThreshold)
    {
        int livesGained = newThreshold - oldThreshold;
        for (int i = 0; i < livesGained; i++)
        {
            gainLife();
        }
        lastLifeThreshold = newThreshold * 100;
    }
}

/**
 * @brief Recupere une piece.
 * @details Si le joueur a plus de 100 il gagne une vie.
 */
void Player::getCoins()
{
    coins++;
    addScore(10);
    if (coins > 100)
    {
        gainLife();
        coins = 0;
    }
}

/**
 * @brief Perd une vie
 * @details Si le joueur na plus de vies, affiche "Game Over !
 */

void Player::loseLife()
{
    lives--;
    if (lives <= 0)
    {
        isDead = true;
        std::cout << characterName << " lost all lives! Game over!" << std::endl;
    }
    else
    {
        isDead = true;
    }
}

/**
 * @brief Gagne une vie
 * @details Le nombre de vies est affiche
 */
void Player::gainLife()
{
    lives++;
}

/**
 * @brief Recupere le nombre de vies du joueur
 */
int Player::getLives() const
{
    return lives;
}

/**
 * @brief Mort du joueur
 * @details Le joueur est mort si il est deja mort et si le timer de collision est inferieur a 0
 * Le joueur perd une vie quand il meurt
 */
void Player::die()
{
    if (!isDead && hitTimer <= 0)
    {
        deathX = sprite.getPosition().x;
        deathY = sprite.getPosition().y;

        isDead = true;
        loseLife();
    }
}

/**
 * @brief Fait grandir le joueur
 * @details Le joueur est grand si il a la taille normale
 * La taille du joueur est modifiee
 */
void Player::grow()
{
    big = true;
    sprite.setScale(1.2f, 1.2f);
    sprite.setPosition(sprite.getPosition().x, 480.0f);
}

/**
 * @brief Fait retrecir le joueur
 * @details Le joueur est petit si il est grand
 * La taille du joueur est modifiee
 */
void Player::shrink()
{
    if (hasFirePower)
    {
        hasFirePower = false;
        big = true;
        sprite.setTexture(normalTexture);
        sprite.setScale(1.2f, 1.2f);

        sprite.setPosition(sprite.getPosition().x, 480.0f);

        hitTimer = 120;

        std::cout << characterName << " lost fire power!" << std::endl;
    }

    else if (big)
    {
        big = false;
        sprite.setScale(0.7f, 0.7f);
        sprite.setPosition(sprite.getPosition().x, 500.0f);
        hitTimer = 120;
    }

    else
    {
        die();
        std::cout << characterName << " was already small and died!" << std::endl;
    }
}

/**
 * @brief Verifie si le joueur est grand
 */
bool Player::isBig() const
{
    return big;
}

/**
 * @brief Verifie si le joueur est petit
 */
bool Player::isSmall() const
{

    return sprite.getScale().x < 1.0f;
}

/**
 * @brief Verifie si le joueur est touche
 */
bool Player::isHit() const
{

    return hitTimer > 0;
}

/**
 * @brief Verifie si le joueur est invincible
 */
bool Player::isInvincible() const
{

    return hitTimer > 0;
}

/**
 * @brief Gère l'animation du player (Mario ou Luigi)
 * @details Cette fonction met à jour l'animation en fonction du mouvement du personnage, des pouvoirs et des directions
 */
void Player::animate()
{
    //incrémentation du compteur de frames
    frameCounter++; 
    //si compteur atteint 40 -> réinitialisation du compteur
    if (frameCounter >= 40)
    {
        frameCounter = 0;

        //passage à la frame suivante en fonction du mouvement
        currentFrame = (currentFrame + 1) % (movingLeft ? (characterName == "luigi" ? 3 : 3) : 4);
    }

    //si joueur a le pouvoir de feu
    if (hasFirePower)
    {
        //si joueur se déplace vers droite
        if (movingRight)
        {
            //si sprite orienté à gauche
            if (sprite.getScale().x < 0)
            {
                //mettre sprite à droite 
                sprite.setScale(std::abs(sprite.getScale().x), sprite.getScale().y);
            }
        }
        //si joueur se déplace vers gauche 
        else if (movingLeft)
        {
            //si sprite orienté à gauche 
            if (sprite.getScale().x > 0)
            {
                //mettre sprite à gauche
                sprite.setScale(-sprite.getScale().x, sprite.getScale().y);
            }
        }
    }
    else //si joueur pas pouvoir de feu
    {
        //si joueur mario
        if (characterName == "Mario")
        {
            //si déplacement droite 
            if (movingRight)
            {
                //texture Mario allant vers droite
                sprite.setTextureRect(sf::IntRect(8 + currentFrame * 28, 139, 26, 47));
                //si sprite orienté à gauche
                if (sprite.getScale().x < 0)
                {
                    //mettre sprite à droite 
                    sprite.setScale(std::abs(sprite.getScale().x), sprite.getScale().y);
                }
            }
            //si déplacement gauche
            else if (movingLeft)
            {
                //texture Mario allant vers gauche
                sprite.setTextureRect(sf::IntRect(202 + currentFrame * 32, 191, 26, 47));
                //si sprite orienté à gauche
                if (sprite.getScale().x < 0)
                {
                    //mettre sprite à droite 
                    sprite.setScale(std::abs(sprite.getScale().x), sprite.getScale().y);
                }
            }
            else //si mario ne se déplace pas
            {
                sprite.setTextureRect(sf::IntRect(8, 139, 28, 47)); //textue statique Mario

                //si sprite orienté à gauche
                if (sprite.getScale().x < 0) 
                {
                    //mettre sprite à droite
                    sprite.setScale(std::abs(sprite.getScale().x), sprite.getScale().y);
                }
            }
        }
        else if (characterName == "Luigi")
        {
            if (movingRight)
            {
                sprite.setTextureRect(sf::IntRect(8 + currentFrame * 28, 191, 24, 47));

                if (sprite.getScale().x < 0)
                {
                    sprite.setScale(std::abs(sprite.getScale().x), sprite.getScale().y);
                }
            }
            else if (movingLeft)
            {
                sprite.setTextureRect(sf::IntRect(262 + currentFrame * 25, 4, 24, 47));
                if (sprite.getScale().x < 0)
                {
                    sprite.setScale(std::abs(sprite.getScale().x), sprite.getScale().y);
                }
            }
            else
            {
                sprite.setTextureRect(sf::IntRect(8, 191, 28, 47));
                if (sprite.getScale().x < 0)
                {
                    sprite.setScale(std::abs(sprite.getScale().x), sprite.getScale().y);
                }
            }
        }
    }
}

void Player::collectEtoile()
{
    std::cout << characterName << " est maintenant avec le pouvoir étoile !" << std::endl;
    currentSpeed = baseSpeed * 3;
    mouvement.setSpeed(currentSpeed);

    isStarPowered = true;
    starPowerFrames = 1800;
}

/**
 * @brief Le joueur collecte une fleur de feu
 * @details Le joueur a maintenant le pouvoir de tirer des boules de feu
 * La texture du joueur est modifiee
 */
void Player::collectFireFlower()
{
    hasFirePower = true;
    big = true;
    sprite.setTexture(fireTexture);
    sprite.setTextureRect(sf::IntRect(0, 0, fireTexture.getSize().x, fireTexture.getSize().y));
    float scaleValue = 0.15f;
    float currentScaleX = sprite.getScale().x;
    float directionMultiplier = (currentScaleX < 0) ? -1.0f : 1.0f;
    sprite.setScale(scaleValue * directionMultiplier, scaleValue);
}

/**
 * @brief Recupere la position precedente du joueur
 */
sf::Sprite &Player::getSprite()
{
    return sprite;
}

/**
 * @brief Verifie si le joueur a gagne
 * @param flag Drapeau de fin de niveau
 * @return true si le joueur a gagne sinon false
 */
bool Player::checkWin(const sf::Sprite &flag)
{
    float playerX = sprite.getPosition().x + sprite.getGlobalBounds().width / 2;
    float playerY = sprite.getPosition().y + sprite.getGlobalBounds().height / 2;
    float flagX = flag.getPosition().x;
    float flagY = flag.getPosition().y;
    if (std::abs(playerX - flagX) < 10.0f && std::abs(playerY - flagY) < 15.0f)
    {
        std::cout << "Victoire !" << std::endl;
        return true;
    }
    return false;
}

/**
 * @brief Le joueur tire une boule de feu
 * @details La boule de feu est tiree si le joueur a le pouvoir de tirer des boules de feu et si le cooldown est a 0
 */
void Player::shootFireball()
{
    if (hasFirePower && fireballCooldown == 0)
    {
        float direction = movingLeft ? -1.0f : 1.0f;
        float offsetX = (direction > 0) ? sprite.getGlobalBounds().width : 0;
        float offsetY = sprite.getGlobalBounds().height / 3;
        fireballs.emplace_back(sprite.getPosition().x + offsetX,
                               sprite.getPosition().y + offsetY,
                               direction);
        fireballCooldown = 15;
    }
}

/**
 * @brief Met a jpur fireballs
 * @param blocks Les blocs avec lesquels verifier la collision
 * @details Cette methode met a jour les boules de feu
 */
void Player::updateFireballs(const std::vector<sf::Sprite> &blocks)
{
    for (auto &fireball : fireballs)
    {
        fireball.update(blocks);
    }

    fireballs.erase(
        std::remove_if(fireballs.begin(), fireballs.end(),
                       [](const Fireball &fireball)
                       { return !fireball.isActive(); }),
        fireballs.end());
}

/**
 * @brief Dessine les boules de feu
 * @param window Fenetre de rendu
 */
void Player::drawFireballs(sf::RenderWindow &window)
{
    for (auto &fireball : fireballs)
    {
        fireball.draw(window);
    }
}

/**
 * @brief Perdre le pouvir du feu
 * @details Le joueur perd le pouvoir de tirer des boules de feu
 * La texture du joueur est modifiee
 */
void Player::loseFirePower()
{
    if (hasFirePower)
    {
        hasFirePower = false;
        sprite.setTexture(normalTexture);
        if (big)
        {
            sprite.setScale(0.15f, 0.15f);
        }
        else
        {
            sprite.setScale(0.1f, 0.1f);
        }
    }
}

/**
 * @brief Le joueur reviens a la partie sil est mort et a des vies restantes
 * @details Le joueur reviens a la partie si il est mort et a des vies restantes
 * La position du joueur est modifiee
 * Le joueur est marque comme vivant
 */
void Player::respawn()
{
    if (lives > 0 && isDead)
    {
        sprite.setPosition(deathX, deathY - 50);

        isDead = false;
        movingRight = false;
        movingLeft = false;

        if (hasFirePower)
        {
            sprite.setTexture(fireTexture);

            float scaleValue = 0.15f;
            sprite.setScale(scaleValue, scaleValue);

            sprite.setTextureRect(sf::IntRect(0, 0, fireTexture.getSize().x, fireTexture.getSize().y));

            std::cout << characterName << " respawned with fire power at death position!" << std::endl;
        }
        else
        {
            sprite.setTexture(normalTexture);

            if (characterName == "Mario")
            {
                sprite.setTextureRect(sf::IntRect(8, 139, 28, 47));
            }
            else if (characterName == "Luigi")
            {
                sprite.setTextureRect(sf::IntRect(8, 191, 28, 47));
            }

            if (big)
            {
                sprite.setScale(1.2f, 1.2f);
            }
            else
            {
                sprite.setScale(0.7f, 0.7f);
            }
        }

        fireballs.clear();

        hitTimer = 180;

        sprite.setColor(sf::Color(255, 255, 255, 180));

        std::cout << characterName << " respawned at death position (" << deathX << ", " << deathY
                  << ")! Remaining lives: " << lives << std::endl;
    }
    else
    {
        std::cout << characterName << " cannot respawn: lives=" << lives << ", isDead=" << isDead << std::endl;
    }
}

/**
 * @brief Met a jour la position initiale du joueur
 * @param x Position horizontale initiale du joueur
 * @param y Position verticale initiale du joueur
 */
void Player::updateInitialPosition(float x, float y)
{
    initialX = x;
    initialY = y;
}

/**
 * @brief Verifie si le joueur doit revenir dans la partie
 * @return true si le joueur doit revenir dans la partie sinon false
 */
bool Player::shouldRespawn() const
{
    return isDead && lives > 0;
}