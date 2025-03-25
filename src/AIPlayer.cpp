#include "Headers/AIPlayer.hpp"
#include <iostream>
#include <cmath>
#include <algorithm>

/**
 * @brief Constructeur de la classe AIPlayer
 * @param character Pointeur vers le joueur controle par l'IA
 * @param enemies Vector contenant les pointeurs vers les ennemis du jeu
 */
AIPlayer::AIPlayer(Player *character, const std::vector<std::unique_ptr<Enemy>> &enemies)
    : character(character),
      decisionCooldown(0),
      currentAction("idle"),
      actionDuration(0),
      jumpTimer(0),
      m_shouldJump(false),
      m_shouldMoveRight(true),
      m_shouldMoveLeft(false),
      m_shouldShootFireball(false)
{
    // Copie des references aux ennemis
    for (const auto &enemy : enemies)
    {
        this->enemies.push_back(enemy.get());
    }

    // Initialisation du generateur de nombres aleatoires avec une seed vraiment aleatoire
    // pour garantir un comportement different a chaque execution
    std::random_device rd;
    rng = std::mt19937(rd());
}

/**
 * @brief Met a jour les actions de l'IA en fonction de letat du jeu
 * @param groundTiles Vector contenant les sprites des blocs de sol
 * @param pipes Vector contenant les sprites des tuyaux
 * @param flag Sprite du drapeau de fin de niveau
 * @details
 * 1. Gestion du temps de refroidissement des decisions
 * 2. Prise de decision si necessaire
 * 3. Traitement des situations specifiques dans cet ordre de priorite:
 *    - Eviter les ennemis (priorite haute)
 *    - Eviter les obstacles (priorite moyenne)
 *    - Se diriger vers le drapeau (priorite basse)
 *    - Detecter et eviter les trous (priorite tres haute)
 * 4. Execution des actions decidees
 */
void AIPlayer::update(const std::vector<sf::Sprite> &groundTiles,
                      const std::vector<sf::Sprite> &pipes,
                      const sf::Sprite &flag)
{
    // Decrementation du compte a rebours avant la prochaine decision
    if (decisionCooldown > 0)
    {
        decisionCooldown--;
    }

    // Si le temps de refroidissement est ecoule prendre une nouvelle decision
    if (decisionCooldown <= 0)
    {
        makeDecision();
    }

    // Application des comportements reactifs avec ordre de priorite
    avoidEnemies();           // Priorite 1: Survie en evitant les ennemis
    avoidObstacles(groundTiles, pipes); // Priorite 2: Eviter les obstacles statiques
    moveTowardsFlag(flag);    // Priorite 3: Progression vers l'objectif

    // Detection des trous - priorite maximale qui ecrase les autres decisions
    if (detectGapAhead(groundTiles))
    {
        m_shouldJump = true;
        m_shouldMoveRight = true; // Force le mouvement vers la droite pour franchir le trou
    }

    // Application des decisions prises aux actions du personnage
    executeAction();

    // Gestion du timer de saut pour eviter les sauts multiples
    if (jumpTimer > 0)
    {
        jumpTimer--;
        m_shouldJump = (jumpTimer > 0);
    }

    // Utilisation occasionnelle du pouvoir de feu si disponible
    if (character->hasFirePowerActive() && std::bernoulli_distribution(0.01)(rng))
    {
        m_shouldShootFireball = true;
    }
}

/**
 * @brief Prend une decision aleatoire pour l'action a effectuer
 * @details
 * - 60% chance d'aller a droite (progression dans le niveau)
 * - 10% chance d'aller a gauche (exploration/evitement)
 * - 20% chance de sauter (franchissement d'obstacles)
 * - 10% chance de rester immobile (pause/observation)
 */
void AIPlayer::makeDecision()
{
    std::vector<std::string> actions = {"move_right", "move_left", "jump", "idle"};
    std::vector<double> weights = {0.6, 0.1, 0.2, 0.1};

    std::discrete_distribution<> d(weights.begin(), weights.end());
    currentAction = actions[d(rng)];

    // Duree aleatoire de l'action pour simuler un comportement humain
    actionDuration = std::uniform_int_distribution<>(minActionDuration, maxActionDuration)(rng);
    decisionCooldown = actionDuration;

    // Conversion de la decision en drapeaux d'action
    m_shouldMoveRight = (currentAction == "move_right");
    m_shouldMoveLeft = (currentAction == "move_left");
    m_shouldJump = (currentAction == "jump");
}

/**
 * @brief Execute l'action determinee par makeDecision
 */
void AIPlayer::executeAction()
{
    // Execution du saut si necessaire et possible
    if (m_shouldJump && !character->isInAir())
    {
        character->jump();
        jumpTimer = jumpTimer > 0 ? jumpTimer : 10;
    }

    // Application des mouvements lateraux
    character->setMovingRight(m_shouldMoveRight);
    character->setMovingLeft(m_shouldMoveLeft);

    // Tir de boule de feu et reinitialisation immediate du drapeau
    // pour eviter de tirer en continu
    if (m_shouldShootFireball)
    {
        character->shootFireball();
        m_shouldShootFireball = false;
    }
}

/**
 * @brief Evite les ennemis en sautant ou en se deplacant
 * 
 * @details
 * 1. Calcul d'une portee de detection variable selon la taille du joueur
 * 2. Pour chaque ennemi dans cette portee:
 *    - Si ennemi a droite et proche: tenter un saut ou reculer
 *    - Si ennemi a gauche et proche: avancer et sauter si necessaire
 *    - Si ennemi tres proche: sauter et s'eloigner
 * 3. Ajout d'un comportement aleatoire pour eviter la predictibilite
 */
void AIPlayer::avoidEnemies()
{
    bool enemyDetected = false;

    sf::Vector2f originalPosition = character->getPosition();
    sf::FloatRect characterBounds = character->getbounds();

    // Distance de detection augmentee si le joueur est petit 
    float detectionRange = character->isSmall() ? enemyDetectionRange * 1.5f : enemyDetectionRange;

    for (const auto enemy : enemies)
    {
        if (!enemy->isAlive())
            continue;

        sf::FloatRect enemyBounds = enemy->getBounds();
        float distanceX = enemyBounds.left - characterBounds.left;
        float distanceY = enemyBounds.top - characterBounds.top;
        float distance = std::sqrt(distanceX * distanceX + distanceY * distanceY);

        // Ennemi dans la zone de detection
        if (distance < detectionRange)
        {
            enemyDetected = true;

            // Ennemi a droite et proche
            if (distanceX > 0 && distanceX < 150)
            {
                if (std::abs(distanceY) < 80) // A peu pres au meme niveau
                {
                    // Sauter si tres proche
                    if (distanceX < 80 && !character->isInAir() && jumpTimer <= 0)
                    {
                        m_shouldJump = true;
                        jumpTimer = 20;
                    }

                    // Utiliser boule de feu si possible
                    if (character->hasFirePowerActive())
                    {
                        m_shouldShootFireball = true;
                    }

                    // 40% de chance de reculer 
                    if (std::bernoulli_distribution(0.4)(rng))
                    {
                        m_shouldMoveLeft = true;
                        m_shouldMoveRight = false;
                    }
                }
            }
            // Ennemi a gauche et proche
            else if (distanceX < 0 && distanceX > -150)
            {
                if (std::abs(distanceY) < 80) // A peu pres au meme niveau
                {
                    // Sauter si tres proche
                    if (distanceX > -80 && !character->isInAir() && jumpTimer <= 0)
                    {
                        m_shouldJump = true;
                        jumpTimer = 20;
                    }

                    // Avancer pour esquiver 
                    m_shouldMoveRight = true;
                    m_shouldMoveLeft = false;

                    // Utiliser boule de feu si possible
                    if (character->hasFirePowerActive())
                    {
                        m_shouldShootFireball = true;
                    }
                }
            }

            // Ennemi extremement proche: reaction d'urgence - saut et eloignement
            if (distance < 50 && !character->isInAir())
            {
                m_shouldJump = true;
                jumpTimer = 25;

                // S'eloigner de l'ennemi
                m_shouldMoveRight = (distanceX < 0);
                m_shouldMoveLeft = (distanceX > 0);
            }
        }
    }


    if (enemyDetected && !character->isInAir() && std::bernoulli_distribution(0.3)(rng))
    {
        m_shouldJump = true;
        jumpTimer = 15;
    }

    // Correction de position si le joueur sort de l'ecran verticalement
    if (character->getPosition().y > 600 || character->getPosition().y < 0)
    {
        character->setPosition(character->getPosition().x, originalPosition.y);
    }

    // Correction de l'opacite pour eviter les problemes d'affichage
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
 * 
 * @details 
 * - Creation d'une zone "virtuelle" devant le joueur
 * - Verification des collisions potentielles avec cette zone
 * - Declenchement du saut avant la collision reelle
 */
void AIPlayer::avoidObstacles(const std::vector<sf::Sprite> &obstacles, const std::vector<sf::Sprite> &pipes)
{
    sf::FloatRect characterBounds = character->getbounds();

    // Creation d'une zone de detection devant le joueur
    sf::FloatRect aheadBounds = characterBounds;
    aheadBounds.left += (m_shouldMoveRight ? 50.0f : -50.0f);

    // Verification des collisions avec les obstacles standard
    for (const auto &obstacle : obstacles)
    {
        if (aheadBounds.intersects(obstacle.getGlobalBounds()))
        {
            m_shouldJump = true;
            jumpTimer = 15;
            break;
        }
    }

    // Verification des collisions avec les tuyaux
    for (const auto &pipe : pipes)
    {
        if (aheadBounds.intersects(pipe.getGlobalBounds()))
        {
            m_shouldJump = true;
            jumpTimer = 15;
            break;
        }
    }
}

/**
 * @brief Detecte un trou devant le joueur
 * @param groundTiles Vector contenant les sprites des blocs de sol
 * @return true si un trou est detecte devant le joueur
 * 
 * @details 
 * 1. Calcul d'un point de verification a une certaine distance devant le joueur
 * 2. Verification de la presence de blocs de sol a ce point
 * 3. Si aucun bloc n'est trouve, c'est qu'il y a un trou
 */
bool AIPlayer::detectGapAhead(const std::vector<sf::Sprite> &groundTiles)
{
    sf::Vector2f pos = character->getPosition();
    float charBottom = pos.y + character->getbounds().height;
    float checkDistance = gapDetectionRange;

    // Calcul du point de verification en fonction de la direction
    sf::Vector2f checkPoint(pos.x + (m_shouldMoveRight ? checkDistance : -checkDistance), charBottom + 30);
    bool groundFound = false;
    
    // Recherche d'une tuile de sol a la position de verification
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

    // Si aucun sol n'est trouve, c'est un trou
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
 * @details de verifier rapidement la proximite d'ennemis sans calculer toutes les distances.
 * Utilisee pour des decisions rapides.
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
 * @details
 * - Si le drapeau est a droite, 80% de chance d'aller vers la droite
 * - Les 20% restants permettent d'autres actions comme sauter ou eviter des obstacles
 */
void AIPlayer::moveTowardsFlag(const sf::Sprite &flag)
{
    float flagX = flag.getPosition().x;
    float characterX = character->getPosition().x;
    float distanceToFlag = flagX - characterX;

    if (distanceToFlag > 0)
    {
        // 80% de chance d'aller vers le drapeau
        if (std::bernoulli_distribution(0.8)(rng))
        {
            m_shouldMoveRight = true;
            m_shouldMoveLeft = false;
        }
    }
}

/**
 * @brief Met a jour les references des ennemis
 * @param newEnemies Vector contenant les pointeurs vers les ennemis du jeu
 */
void AIPlayer::updateEnemyReferences(std::vector<std::unique_ptr<Enemy>> &newEnemies)
{
    // Reinitialisation complete de la liste d'ennemis
    enemies.clear();

    // Copie des nouvelles references
    for (const auto &enemy : newEnemies)
    {
        enemies.push_back(enemy.get());
    }

    std::cout << "AI updated with " << enemies.size() << " enemies to track" << std::endl;
}
