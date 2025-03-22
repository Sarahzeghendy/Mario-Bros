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

//Player::Player(const std::string& texturePath, float x, float y, float speed, sf::Keyboard::Key right,sf::Keyboard::Key left, sf::Keyboard::Key jump,const std::string& characterType ) 
  //  : mouvement(sprite, speed), rightKey(right), leftKey(left), jumpKey(jump), lives(3), coins(0), big(true), isDead(false), movingRight(false), movingLeft(false), currentFrame(0), frameCounter(0), characterType(characterType)
    

Player::Player(const std::string& texturePath, const std::string& name, float x, float y, float speed, 
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
      hitTimer(0), // Initialise hitTimer to 0
      baseSpeed(speed), // Stocke la vitesse initiale
      currentSpeed(speed), // Initialise aussi la vitesse actuelle
      score(0),
      initialX(x),
      initialY(y),
      deathX(x),    // Initialize death position to match initial position
      deathY(y),
      lastLifeThreshold(0)  // Initialize the last life threshold at 0
{
 
    if (!normalTexture.loadFromFile(texturePath)) 
    {
        std::cerr << "Erreur : Impossible de charger " << texturePath << std::endl;
    }

    std::string fireTexturePath;
    if (characterName == "Mario") {
        fireTexturePath = "images/mario_fire.png";
    } else if (characterName == "Luigi") {
        fireTexturePath = "images/luigi_fire.png"; 
    } else {
        
        fireTexturePath = "images/mario_fire.png";
    }
    
    if (!fireTexture.loadFromFile(fireTexturePath)) 
    {
        std::cerr << "Erreur : Impossible de charger " << fireTexturePath << std::endl;
        fireTexture = normalTexture; 
    }
    else {
        std::cout << "Fire texture loaded successfully: " << fireTexturePath << std::endl;
        // Print the size of the texture to verify dimensions
        std::cout << "Fire texture size: " << fireTexture.getSize().x << "x" << fireTexture.getSize().y << std::endl;
    }
    
    sprite.setTexture(normalTexture);
    sprite.setScale(1.2f, 1.2f);
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
void Player::update(const std::vector<sf::Sprite>& blocks, const std::vector<sf::Sprite>& pipes, const std::vector<sf::Sprite>& questionBlocks) {
    if (isDead) return;

    // Decrease hit invincibility timer if it's active
    if (hitTimer > 0) {
        hitTimer--;
        
        // Flash the sprite to show invincibility
        if (hitTimer % 10 < 5) {
            sprite.setColor(sf::Color(255, 255, 255, 128)); // Semi-transparent
        } else {
            sprite.setColor(sf::Color(255, 255, 255, 255)); // Fully visible
        }
        
        // Reset to fully visible when invincibility ends
        if (hitTimer == 0) {
            sprite.setColor(sf::Color(255, 255, 255, 255));
        }
    }

    if (fireballCooldown > 0) {
        fireballCooldown--;
    }

    movingRight = sf::Keyboard::isKeyPressed(rightKey);
    movingLeft = sf::Keyboard::isKeyPressed(leftKey);

    updateFireballs(blocks);

    auto [canMoveRight, canMoveLeft] = mouvement.blockMovement(blocks, pipes, questionBlocks);

    if (sf::Keyboard::isKeyPressed(rightKey) && canMoveRight) {
        mouvement.moveRight();
        movingRight = true;  
        movingLeft = false;
    }

    if (sf::Keyboard::isKeyPressed(leftKey) && canMoveLeft) {
        mouvement.moveLeft();
        movingLeft = true;  
        movingRight = false;
    }

    if (sf::Keyboard::isKeyPressed(jumpKey)) {
        mouvement.jump();
    }

    if (isStarPowered) 
    {
        starPowerFrames--; // Décrémentation du compteur

        if (starPowerFrames <= 0)
        {
            
            // Rétablir la vitesse normale
            currentSpeed = baseSpeed;
            mouvement.setSpeed(currentSpeed);
            isStarPowered = false;
            std::cout << characterName << " vient de perdre le pouvoir étoile !" << std::endl;
        }
    }

    applyGravity(blocks, pipes, questionBlocks);

    // Call animate regardless of fire power status
    animate();
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
void Player::applyGravity(const std::vector<sf::Sprite>& blocks, const std::vector<sf::Sprite>& pipes, const std::vector<sf::Sprite>& questionBlocks) {
    mouvement.applyGravity( blocks, pipes, questionBlocks);
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
 * @brief Ajoute des points au score et vérifie les seuils pour gagner des vies
 * @param points Nombre de points à ajouter
 */
void Player::addScore(int points) {
    int oldScore = score;
    score += points;
    
    // Check if player has crossed a 100-point threshold
    int oldThreshold = oldScore / 100;
    int newThreshold = score / 100;
    
    if (newThreshold > oldThreshold) {
        // Player has crossed at least one 100-point threshold
        int livesGained = newThreshold - oldThreshold;
        for (int i = 0; i < livesGained; i++) {
            gainLife();
            std::cout << characterName << " gained a life from score! Lives: " << lives << std::endl;
        }
        lastLifeThreshold = newThreshold * 100;
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
    addScore(10);  // Replace direct score addition with the new method

    if (coins > 100) 
    {
        gainLife();
        coins = 0;
    }
}

/**
 * @brief Perd une vie.
 * @details Si le joueur n'a plus de vies, affiche "Game Over !".
 */

void Player::loseLife() {
    lives--;
    std::cout << characterName << " lost a life! Remaining lives: " << lives << std::endl;
    
    if (lives <= 0) {
        // No more lives, player is permanently dead
        isDead = true;
        std::cout << characterName << " lost all lives! Game over!" << std::endl;
    } else {
        // Player still has lives, mark for respawn
        isDead = true;
    }
}

/**
 * @brief Gagne une vie.
 * @details Le nombre de vies est affiché.
 */
void Player::gainLife() {
    lives++;
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
    if (!isDead && hitTimer <= 0) { // Only die if not already dead and not invincible
        // Store current position as death position
        deathX = sprite.getPosition().x;
        deathY = sprite.getPosition().y;
        
        isDead = true;
        loseLife(); // Lose a life when player dies
        std::cout << characterName << " est mort ! Vies restantes : " << lives << std::endl;
    }
}

/**
 * @brief Fait grandir le joueur.
 */
void Player::grow() 
{
    big = true;  
    sprite.setScale(1.2f, 1.2f); 
    sprite.setPosition(sprite.getPosition().x, 480.0f);
}

/**
 * @brief Fait rétrécir le joueur.
 */
void Player::shrink() 
{
    // First check if player has fire power - losing fire power takes precedence
    if (hasFirePower) {
        hasFirePower = false;
        
        // Keep big state but revert to normal texture
        big = true;
        sprite.setTexture(normalTexture);
        
        // Set proper scale for big Mario/Luigi
        sprite.setScale(1.2f, 1.2f);
        
        // Reposition if needed
        sprite.setPosition(sprite.getPosition().x, 480.0f);
        
        // Set invincibility timer
        hitTimer = 120;
        
        std::cout << characterName << " lost fire power!" << std::endl;
    }
    // If player is big (but doesn't have fire power), shrink them
    else if (big) {
        big = false;
        sprite.setScale(0.7f, 0.7f); 
        sprite.setPosition(sprite.getPosition().x, 500.0f);
        hitTimer = 120;
        
        std::cout << characterName << " shrank to small size!" << std::endl;
    } 
    // If player is already small, kill them
    else {
        die();
        std::cout << characterName << " was already small and died!" << std::endl;
    }
}

/**
 * @brief Vérifie si le joueur est grand.
 */
bool Player::isBig() const 
{
    return big;
}

bool Player::isSmall() const {
    // Determine if player is in small state based on sprite size or other indicators
    // This depends on how you're implementing the shrinking mechanic
    return sprite.getScale().x < 1.0f; // Adjust this condition based on your implementation
}

bool Player::isHit() const {
    // Return true if player was recently hit
    return hitTimer > 0;
}

bool Player::isInvincible() const {
    // Return true if player is currently invincible (right after being hit)
    return hitTimer > 0;
}

// Remove this implementation as it's already defined elsewhere
// bool Player::isInAir() const {
//     return mouvement.isInAir();
// }

void Player::animate()
{
    frameCounter++;
    if (frameCounter >= 40)
    {
        frameCounter = 0;
        currentFrame = (currentFrame + 1) % (movingLeft ? (characterName == "luigi" ? 3 : 3) : 4);
    }

    if (hasFirePower) {
        // For fire power, we'll just flip the sprite horizontally based on direction
        // instead of using different animation frames
        if (movingRight) {
            // When moving right, make sure scale is positive (no flip)
            if (sprite.getScale().x < 0) {
                sprite.setScale(std::abs(sprite.getScale().x), sprite.getScale().y);
            }
        } else if (movingLeft) {
            // When moving left, make scale negative (horizontal flip)
            if (sprite.getScale().x > 0) {
                sprite.setScale(-sprite.getScale().x, sprite.getScale().y);
            }
        }
        // No need to change texture rect for fire power as we're using the full texture
    } else {
        // Original animation for non-fire power state
        if (characterName == "Mario") {
            if (movingRight) {
                sprite.setTextureRect(sf::IntRect(8 + currentFrame * 28, 139, 26, 47));
                // Ensure no flip
                if (sprite.getScale().x < 0) {
                    sprite.setScale(std::abs(sprite.getScale().x), sprite.getScale().y);
                }
            } else if (movingLeft) {
                sprite.setTextureRect(sf::IntRect(202 + currentFrame * 32, 191, 26, 47));
                // Ensure no flip as we're using left-facing frames
                if (sprite.getScale().x < 0) {
                    sprite.setScale(std::abs(sprite.getScale().x), sprite.getScale().y);
                }
            } else { // Standing still
                sprite.setTextureRect(sf::IntRect(8, 139, 28, 47));
                // Default to right-facing
                if (sprite.getScale().x < 0) {
                    sprite.setScale(std::abs(sprite.getScale().x), sprite.getScale().y);
                }
            }
        } else if (characterName == "Luigi") {
            if (movingRight) {
                sprite.setTextureRect(sf::IntRect(8 + currentFrame * 28, 191, 24, 47));
                // Ensure no flip
                if (sprite.getScale().x < 0) {
                    sprite.setScale(std::abs(sprite.getScale().x), sprite.getScale().y);
                }
            } else if (movingLeft) {
                sprite.setTextureRect(sf::IntRect(262 + currentFrame * 25, 4, 24, 47));
                // Ensure no flip as we're using left-facing frames
                if (sprite.getScale().x < 0) {
                    sprite.setScale(std::abs(sprite.getScale().x), sprite.getScale().y);
                }
            } else { // Standing still
                sprite.setTextureRect(sf::IntRect(8, 191, 28, 47));
                // Default to right-facing
                if (sprite.getScale().x < 0) {
                    sprite.setScale(std::abs(sprite.getScale().x), sprite.getScale().y);
                }
            }
        }
    }
}

//fonction pour l'étoile
void Player::collectEtoile()
{
    std::cout << characterName << " est maintenant avec le pouvoir étoile !" << std::endl;
    // Doubler la vitesse en fonction de l'état actuel
    currentSpeed = baseSpeed * 2;
    mouvement.setSpeed(currentSpeed);

    // Activer l'état étoilé
    isStarPowered = true;
    starPowerFrames = 1800; // Environ 10 secondes si 60 FPS
}


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

sf::Sprite& Player::getSprite() {
    return sprite;
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

void Player::updateFireballs(const std::vector<sf::Sprite>& blocks)
{
    for (auto &fireball : fireballs) {
        fireball.update(blocks);
    }

    fireballs.erase(
        std::remove_if(fireballs.begin(), fireballs.end(),
                       [](const Fireball &fireball) { return !fireball.isActive(); }),
        fireballs.end());
}

void Player::drawFireballs(sf::RenderWindow &window)
{
    for (auto &fireball : fireballs) {
        fireball.draw(window);
    }
}

void Player::loseFirePower()
{
    if (hasFirePower) {
        hasFirePower = false;
        sprite.setTexture(normalTexture);
        if (big) {
            sprite.setScale(0.15f, 0.15f);
        } else {
            sprite.setScale(0.1f, 0.1f);
        }
    }
}

void Player::respawn() {
    if (lives > 0 && isDead) {
        // Reset position to death position instead of initial position
        sprite.setPosition(deathX, deathY);
        
        // Reset state
        isDead = false;
        movingRight = false;
        movingLeft = false;
        
        // Keep fire power but reset texture
        if (hasFirePower) {
            sprite.setTexture(fireTexture);
        } else {
            sprite.setTexture(normalTexture);
        }
        
        // Reset to big size if player had fire power
        if (hasFirePower) {
            big = true;
            float scaleValue = 0.15f;
            sprite.setScale(scaleValue, scaleValue);
        } else {
            // Otherwise reset to normal size - make sure it's visible
            big = false;
            sprite.setScale(0.7f, 0.7f);
        }
        
        // Clear any active fireballs
        fireballs.clear();
        
        // Reset texture rect to standing position
        if (characterName == "Mario") {
            sprite.setTextureRect(sf::IntRect(8, 139, 28, 47));
        } else if (characterName == "Luigi") {
            sprite.setTextureRect(sf::IntRect(8, 191, 28, 47));
        }
        
        // Reset immunity timer for longer period after respawn
        hitTimer = 180; // 3 seconds of immunity after respawn
        
        // Make sure player is visible with temporary invincibility effect
        sprite.setColor(sf::Color(255, 255, 255, 180));
        
        std::cout << characterName << " respawned at death position! Remaining lives: " << lives << std::endl;
    } else {
        std::cout << characterName << " cannot respawn: lives=" << lives << ", isDead=" << isDead << std::endl;
    }
}

void Player::updateInitialPosition(float x, float y) {
    initialX = x;
    initialY = y;
}

bool Player::shouldRespawn() const {
    return isDead && lives > 0;
}