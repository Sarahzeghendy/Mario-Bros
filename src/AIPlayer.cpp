#include "Headers/AIPlayer.hpp"
#include <iostream>
#include <cmath>
#include <algorithm>
/**
 * @brief Constructeur de la classe AIPlayer
 * @param character Pointeur vers le joueur contrôlé par l'IA
 * @param enemies Vector contenant les pointeurs vers les ennemis du jeu
 */
AIPlayer::AIPlayer(Player *character, const std::vector<std::unique_ptr<Enemy>> &enemies)
    : character(character),
      decisionCooldown(0),
      currentAction("idle"),
      actionDuration(0),
      jumpTimer(0),
      shouldJump(false),
      shouldMoveRight(true),
      shouldMoveLeft(false),
      shouldShootFireball(false)

{ // Ajout de la référence aux ennemis
    for (const auto &enemy : enemies)
    {
        this->enemies.push_back(enemy.get());
    }

    std::random_device rd;
    rng = std::mt19937(rd());
}
/**
 * @brief Met à jour les actions de l'IA en fonction de l'état du jeu
 * @param groundTiles Vector contenant les sprites des blocs de sol
 * @param pipes Vector contenant les sprites des tuyaux
 * @param flag Sprite du drapeau de fin de niveau
 * @details Cette méthode est appelée à chaque frame pour mettre à jour les actions de l'IA
 * en fonction de l etat du jeu L'IA prend des decisions aleatoires pour déterminer ses actions et peut sauter
 * pour eviter les obstacles et les ennemis elle peut également tirer des boules de feu si
 * elle possède le pouvoir de feu
 */
void AIPlayer::update(const std::vector<sf::Sprite> &groundTiles,
                      const std::vector<sf::Sprite> &pipes,
                      const sf::Sprite &flag)
{

    if (decisionCooldown > 0)
    {
        decisionCooldown--;
    }

    if (decisionCooldown <= 0)
    {
        makeDecision();
    }

    avoidEnemies();
    avoidObstacles(groundTiles, pipes);

    moveTowardsFlag(flag);

    if (detectGapAhead(groundTiles))
    {
        shouldJump = true;
        shouldMoveRight = true;
    }

    executeAction();

    if (jumpTimer > 0)
    {
        jumpTimer--;
        shouldJump = (jumpTimer > 0);
    }

    if (character->hasFirePowerActive() && std::bernoulli_distribution(0.01)(rng))
    {
        shouldShootFireball = true;
    }
}

/**
 * @brief Prend une decision aleatoire pour laction a effectuer
 * @details Cette methode genere une action aleatoire a effectuer par lIA en fonction de
 * probabilites actions possibles sont move right move left jump et idle
 * Les probabilits de chaque action sont defins dans le vecteur weights
 * La dure de laction est egalement determinee aleatoirement entre minActionDuration et maxActionDuration
 * Une fois laction determinee les variables shouldMoveRight shouldMoveLeft shouldJump shouldShootFireball
 * sont mises a jour en consequence
 */
void AIPlayer::makeDecision()
{

    std::vector<std::string> actions = {"move_right", "move_left", "jump", "idle"};
    std::vector<double> weights = {0.6, 0.1, 0.2, 0.1};

    std::discrete_distribution<> d(weights.begin(), weights.end());
    currentAction = actions[d(rng)];

    actionDuration = std::uniform_int_distribution<>(minActionDuration, maxActionDuration)(rng);
    decisionCooldown = actionDuration;

    shouldMoveRight = (currentAction == "move_right");
    shouldMoveLeft = (currentAction == "move_left");
    shouldJump = (currentAction == "jump");

    std::cout << "AI Decision: " << currentAction << " for " << actionDuration << " frames" << std::endl;
}

/**
 * @brief Execute laction determinee par makeDecision
 * @details Cette methode met a jour les variables de daplacement et de saut du joueur en fonction de laction
 * determinee par makeDecision Si laction est de sauter le joueur saute et shouldJump est mis a faux
 * Si l action est de se deplacer a droite -> shouldMoveRight est mis a vrai et shouldMoveLeft a faux
 * Si l action est de se deplacer a gauche -> shouldMoveLeft est mis a vrai et shouldMoveRight a faux
 * Si l action est de tirer une boule de feu -> shouldShootFireball est mis a vrai
 */
void AIPlayer::executeAction()
{

    if (shouldJump && !character->isInAir())
    {
        character->jump();
        std::cout << "Luigi AI executed jump!" << std::endl;
        jumpTimer = jumpTimer > 0 ? jumpTimer : 10;
    }

    character->setMovingRight(shouldMoveRight);
    character->setMovingLeft(shouldMoveLeft);

    std::cout << "Luigi position: (" << character->getPosition().x << ", "
              << character->getPosition().y << ") right="
              << (shouldMoveRight ? "true" : "false")
              << " left=" << (shouldMoveLeft ? "true" : "false")
              << std::endl;

    if (shouldShootFireball)
    {
        character->shootFireball();
        shouldShootFireball = false;
    }
}

/**
 * @brief Evite les ennemis en sautant ou en se deplacant
 * @details Cette methode verifie si un ennemi est proche du joueur et ajuste ses actions en consequence
 * Si un ennemi est detecte a une certaine distance le joueur saute ou se deplace pour eviter l ennemi
 * Si l ennemi est proche le joueur saute et se deplace pour eviter la collision
 * Si le joueur est en dehors de l ecran en y il est replace a sa position initiale
 * Si le joueur est transparent il est rendu opaque
 */
void AIPlayer::avoidEnemies()
{
    bool enemyDetected = false;

    sf::Vector2f originalPosition = character->getPosition();
    sf::FloatRect characterBounds = character->getbounds();

    float detectionRange = character->isSmall() ? enemyDetectionRange * 1.5f : enemyDetectionRange;

    for (const auto enemy : enemies)
    {
        if (!enemy->isAlive())
            continue;

        sf::FloatRect enemyBounds = enemy->getBounds();
        float distanceX = enemyBounds.left - characterBounds.left;
        float distanceY = enemyBounds.top - characterBounds.top;
        float distance = std::sqrt(distanceX * distanceX + distanceY * distanceY);

        if (distance < detectionRange)
        {
            enemyDetected = true;

            if (distanceX > 0 && distanceX < 150)
            {

                if (std::abs(distanceY) < 80)
                {

                    if (distanceX < 80 && !character->isInAir() && jumpTimer <= 0)
                    {
                        shouldJump = true;
                        jumpTimer = 20;
                    }

                    if (character->hasFirePowerActive())
                    {
                        shouldShootFireball = true;
                    }

                    if (std::bernoulli_distribution(0.4)(rng))
                    {
                        shouldMoveLeft = true;
                        shouldMoveRight = false;
                    }
                }
            }

            else if (distanceX < 0 && distanceX > -150)
            {

                if (std::abs(distanceY) < 80)
                {

                    if (distanceX > -80 && !character->isInAir() && jumpTimer <= 0)
                    {
                        shouldJump = true;
                        jumpTimer = 20;
                    }

                    shouldMoveRight = true;
                    shouldMoveLeft = false;

                    if (character->hasFirePowerActive())
                    {
                        shouldShootFireball = true;
                    }
                }
            }

            if (distance < 50 && !character->isInAir())
            {
                shouldJump = true;
                jumpTimer = 25;

                shouldMoveRight = (distanceX < 0);
                shouldMoveLeft = (distanceX > 0);
            }
        }
    }

    if (enemyDetected && !character->isInAir() && std::bernoulli_distribution(0.3)(rng))
    {
        shouldJump = true;
        jumpTimer = 15;
    }

    if (character->getPosition().y > 600 || character->getPosition().y < 0)
    {

        character->setPosition(character->getPosition().x, originalPosition.y);
    }

    sf::Color currentColor = character->getSprite().getColor();
    if (currentColor.a < 255)
    {
        sf::Color newColor = currentColor;
        newColor.a = 255;
        character->getSprite().setColor(newColor);
    }
}
/**
 * @brief Evite les obstacles en sautant
 * @param obstacles Vector contenant les sprites des obstacles
 * @param pipes Vector contenant les sprites des tuyaux
 * @details Cette methode verifie si un obstacle est proche du joueur et ajuste ses actions en consequence
 * Si un obstacle est detecte a une certaine distance le joueur saute pour eviter l obstacle
 * Si un tuyau est detecte a une certaine distance le joueur saute pour eviter le tuyau
 */
void AIPlayer::avoidObstacles(const std::vector<sf::Sprite> &obstacles, const std::vector<sf::Sprite> &pipes)
{
    sf::FloatRect characterBounds = character->getbounds();

    sf::FloatRect aheadBounds = characterBounds;
    aheadBounds.left += (shouldMoveRight ? 50.0f : -50.0f);

    for (const auto &obstacle : obstacles)
    {
        if (aheadBounds.intersects(obstacle.getGlobalBounds()))
        {
            shouldJump = true;
            jumpTimer = 15;
            break;
        }
    }

    for (const auto &pipe : pipes)
    {
        if (aheadBounds.intersects(pipe.getGlobalBounds()))
        {
            shouldJump = true;
            jumpTimer = 15;
            break;
        }
    }
}

/**
 * @brief Detecte un trou devant le joueur
 * @param groundTiles Vector contenant les sprites des blocs de sol
 * @return true si un trou est detecte devant le joueur
 * @details Cette methode verifie si un trou est detecte devant le joueur en regardant si un bloc de sol est present
 * a une certaine distance devant le joueur Si aucun bloc de sol nest detecte le joueur saute pour eviter le trou
 * et shouldJump est mis a vrai
 */
bool AIPlayer::detectGapAhead(const std::vector<sf::Sprite> &groundTiles)
{
    sf::Vector2f pos = character->getPosition();
    float charBottom = pos.y + character->getbounds().height;
    float checkDistance = gapDetectionRange;

    sf::Vector2f checkPoint(pos.x + (shouldMoveRight ? checkDistance : -checkDistance), charBottom + 30);
    bool groundFound = false;
    for (const auto &tile : groundTiles)
    {
        sf::FloatRect tileBounds = tile.getGlobalBounds();
        if (checkPoint.x >= tileBounds.left && checkPoint.x <= tileBounds.left + tileBounds.width &&
            std::abs(checkPoint.y - tileBounds.top) < 50)
        {
            groundFound = true;
            break;
        }
    }

    if (!groundFound)
    {
        std::cout << "Gap detected ahead! Jumping!" << std::endl;
        return true;
    }

    return false;
}

/**
 * @brief Verifie si un ennemi est proche du joueur
 * @param distance Distance maximale a laquelle un ennemi est considere proche
 * @return true si un ennemi est proche du joueur
 * @details Cette methode verifie si un ennemi est proche du joueur a une certaine distance
 * Si un ennemi est detecte a une distance inferieure a la distance specifiee la methode retourne vrai
 * Sinon elle retourne faux
 * Cette methode est utilisee pour determiner si le joueur doit eviter les ennemis
 * ou si lIA doit attaquer les ennemis
 */
bool AIPlayer::isEnemyNearby(float distance)
{
    for (const auto enemy : enemies)
    {
        if (!enemy->isAlive())
            continue;

        float distanceX = std::abs(enemy->getBounds().left - character->getPosition().x);
        if (distanceX < distance)
        {
            return true;
        }
    }
    return false;
}

/**
 * @brief Deplace le joueur vers le drapeau de fin de niveau
 * @param flag Sprite du drapeau de fin de niveau
 * @details Cette methode deplace le joueur vers le drapeau de fin de niveau
 * en ajustant ses actions en fonction de la position du drapeau
 * Si le drapeau est a droite du joueur lIA a 80% de chances de se deplacer a droite
 * Si le drapeau est a gauche du joueur lIA a 80% de chances de se deplacer a gauche
 * Si le drapeau est a la meme position que le joueur aucune action nest effectuee
 */
void AIPlayer::moveTowardsFlag(const sf::Sprite &flag)
{

    float flagX = flag.getPosition().x;
    float characterX = character->getPosition().x;
    float distanceToFlag = flagX - characterX;

    if (distanceToFlag > 0)
    {
        if (std::bernoulli_distribution(0.8)(rng))
        {
            shouldMoveRight = true;
            shouldMoveLeft = false;
        }
    }
}

/**
 * @brief Met a jour les references des ennemis
 * @param newEnemies Vector contenant les pointeurs vers les ennemis du jeu
 * @details Cette methode met a jour les references des ennemis en vidant le vecteur enemies
 * et en ajoutant les nouveaux ennemis au vecteur
 * Cette methode est appelee a chaque fois quun ennemi est ajoute ou supprime du jeu
 * pour mettre a jour les actions de lIA
 */
void AIPlayer::updateEnemyReferences(std::vector<std::unique_ptr<Enemy>> &newEnemies)
{

    enemies.clear();

    for (const auto &enemy : newEnemies)
    {
        enemies.push_back(enemy.get());
    }

    std::cout << "AI updated with " << enemies.size() << " enemies to track" << std::endl;
}
