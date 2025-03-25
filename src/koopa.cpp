#include "Headers/enemy.hpp"
#include <iostream>


/**
 * @brief Constructeur de la classe KoopaTroopa
 * @param x Position en x du KoopaTroopa
 * @param y Position en y du KoopaTroopa
 * @details Initialise un KoopaTroopa avec une image et une position donnée, et charge l'image de la carapace
 */
KoopaTroopa::KoopaTroopa(float x, float y)
    : Enemy("images/koopa.png", x, y, 0, 0)  
{
    sprite.setScale(0.15f, 0.15f);
    speed = 0.25f;  
    if (!koopaShell.loadFromFile("images/koopa_closed.png")) 
    {
        std::cerr << "Erreur: Impossible de charger koopa_closed.png\n";
    }
}



/**
 * @brief Met à jour la position et l'état du KoopaTroopa
 * @details Gère le déplacement du KoopaTroopa en fonction de son état (carapace, mouvement, etc.)
 */
void KoopaTroopa::update() 
{
    //si koopatroopa pas vivant -> ne fait rien 
    if (!alive) return;

    //koopatroopa tombe
    if (isFalling)
    {
        fall();
        return;
    }

    //sauvegarde position précédente
    sf::Vector2f oldPosition = sprite.getPosition();

    //si koopatroopa se déplace vers la droite
    if (movingRight) 
    {
        //si dans sa carapace 
        if (inShellState) 
        {
            //se déplace plus vite
            sprite.move(speed * 3, 0);  
        } else 
        {
            //se déplace normalement
            sprite.move(speed, 0);
        }
        //si koopatroopa ne fait pas face à droite, inversion direction
        if (!inShellState && sprite.getScale().x < 0) 
        {
            sprite.setScale(0.15f, 0.15f);
        }
    } 
    else  //déplacement vers gauche
    {
        //Si dans sa carapace
        if (inShellState) 
        {
            //se déplace plus vite
            sprite.move(-speed * 3, 0);  
        } else 
        {
            //se déplace normalement
            sprite.move(-speed, 0);
        }
        //si koopatroopa ne fait pas face à gauche, inversion direction
        if (!inShellState && sprite.getScale().x > 0) 
        {
            sprite.setScale(-0.15f, 0.15f);
        }
    }
    //vérification collisions avec objets (blocs, tuyaux,...)
    handleCollisions(oldPosition);
    
}

/**
 * @brief Gère l'interaction avec le joueur
 * @param player Référence au joueur
 * @details Lorsqu'un joueur entre en collision avec le KoopaTroopa, il subit une action (réduction de taille ou mort)
 */
void KoopaTroopa::interactWithPlayer(Player& player) 
{
    //vérification collision entre koopatroopa et joueur 
    if (sprite.getGlobalBounds().intersects(player.getbounds())) 
    {
        //si koopatroopa dans sa carapace -> rien ne se passe
        if (inShellState) 
        {
            return;
        }
        //si joueur grand
        else if (player.isBig()) 
        {
            //joueur devient petit
            player.shrink();
        } 
        else //si joueur petit
        {
            //joueur meurt
            player.die();
        }
    }
}

/**
 * @brief Gère l'action lorsque KoopaTroopa est sauté dessus
 * @details Lorsque le joueur saute sur KoopaTroopa, cela le met dans un état de carapace ou inverse son mouvement
 */
void KoopaTroopa::onJumpedOn() 
{
    //si koopatroopa est dans sa carapace 
    if (inShellState) 
    {
        //inversion direction
        movingRight = !movingRight;
   
        sf::Vector2f currentPos = sprite.getPosition();
        if (movingRight) 
        {
            sprite.setPosition(currentPos.x + 10.0f, currentPos.y);
        } else 
        {
            sprite.setPosition(currentPos.x - 10.0f, currentPos.y);
        }
     
        speed = 0.7f;
        return;
    }
    
    inShellState = true; //koopatroopa dans sa carapace

    sf::Vector2f originalPosition = sprite.getPosition();

    //décalage léger position pour animation carapace
    originalPosition.y -= 5.0f; 

    //changement sprite pour carapace koopatroopa
    sprite.setTexture(koopaShell, true);  

    sf::Vector2u textureSize = koopaShell.getSize();
    //application texture à la carapace
    sprite.setTextureRect(sf::IntRect(0, 0, textureSize.x, textureSize.y));
    
    sprite.setPosition(originalPosition);
    sprite.setScale(0.12f, 0.12f);
    
    //décision aléatoire pour direction carapace déplacement
    movingRight = (rand() % 2 == 0);  
    
    speed = 0.8f;  // Increased from 0.4f for shell initial speed
    
}

/**
 * @brief Gère l'impact avec une boule de feu
 * @details Lorsque KoopaTroopa est touché par une boule de feu, il meurt
 */
void KoopaTroopa::onFireballHit()
{
   //koopatroopa est tué
    alive = false;
    
    //changement orientation du sprite lorsqu'il est tué
    sprite.setScale(0.15f, -0.15f);  
}

/**
 * @brief Inverse la direction du KoopaTroopa
 * @details Change la direction du KoopaTroopa (gauche ou droite) et ajuste son orientation visuelle
 */
void KoopaTroopa::reverseDirection() 
{
    movingRight = !movingRight;
    
    if (movingRight) //koopatroopa fait face à droite
    {
        sprite.setScale(0.15f, 0.15f);  
    } 
    else //koopatroopa fait face à gauche
    {
        sprite.setScale(-0.15f, 0.15f); 
    }
}

/**
 * @brief Gère les collisions avec les blocs et tuyaux
 * @param oldPosition Position précédente du KoopaTroopa
 * @details Vérifie les collisions avec les blocs et les tuyaux et ajuste la direction de KoopaTroopa si nécessaire
 */
void KoopaTroopa::handleCollisions(const sf::Vector2f& oldPosition) 
{
    //si koopatroopa pas dans sa carapace
    if (!inShellState) 
    {
        //gestion collision de la classe enemy
        Enemy::handleCollisions(oldPosition);
        return;
    }

    //récupère les limites globales du sprite
    sf::FloatRect globalBounds = sprite.getGlobalBounds();
    
    // création rectangles collision pour la droite de KoopaTroopa
    sf::FloatRect rightBounds(
        globalBounds.left + globalBounds.width - 5, 
        globalBounds.top, 
        10, 
        globalBounds.height - 5 
    );
    
    // création rectangles collision pour la gauche de KoopaTroopa
    sf::FloatRect leftBounds(
        globalBounds.left - 5, 
        globalBounds.top, 
        10, 
        globalBounds.height - 5 
    );
    
    //variable pour vérifier si collision a eu lieu
    bool collided = false;
 
    //vérification collisions avec tuyaux
    for (const auto& pipe : currentPipes) 
    {
        sf::FloatRect pipeBounds = pipe.getGlobalBounds();
        float pipeLeft = pipeBounds.left + 60.0f;
        float pipeWidth = 35.4f;
        sf::FloatRect fixedPipeBounds(pipeLeft, pipeBounds.top, pipeWidth, pipeBounds.height);
        
        //si collision détectée avec tuyau
        if (movingRight && rightBounds.intersects(fixedPipeBounds)) 
        {
            collided = true;
            break;
        }
        else if (!movingRight && leftBounds.intersects(fixedPipeBounds)) 
        {
            collided = true;
            break;
        }
    }

    //vérification collisions avec blocs
    if (!collided) 
    {
        for (const auto& block : currentBlocks) 
        {
            sf::FloatRect blockBounds = block.getGlobalBounds();
            
            if (blockBounds.top > globalBounds.top + (globalBounds.height * 0.8f)) {
                continue; //ignorer blocs sous koopatroopa
            }
            //si collision détectée avec bloc
            if (movingRight && rightBounds.intersects(blockBounds)) 
            {
                collided = true;
                break;
            }
            else if (!movingRight && leftBounds.intersects(blockBounds)) 
            {
                collided = true;
                break;
            }
        }
    }
    //si collision a eu lieu -> inversion direction 
    if (collided) 
    {
        //inversion direction
        movingRight = !movingRight;
     
        if (movingRight) 
        {
            sprite.setPosition(oldPosition.x - 15.0f, oldPosition.y); 
        } else 
        {
            sprite.setPosition(oldPosition.x + 15.0f, oldPosition.y);
        }
    }
}
