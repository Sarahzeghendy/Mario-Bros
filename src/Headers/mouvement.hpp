#ifndef MOUVEMENT_HPP
#define MOUVEMENT_HPP

#include <SFML/Graphics.hpp>

/**
 * @class Mouvement
 * @brief Gère les déplacements d'un personnage, y compris le déplacement horizontal, le saut et la gravité.
 */
class Mouvement {
private:

    sf::Sprite& sprite; 
    float speed; 
    bool isJumping; 
    float velocityY; 
    float gravity; 
    float jumpStrength; 
    float groundY; 
    float hangTime; 
    float hangCounter; 

public:
    /**
     * @brief Constructeur de la classe Mouvement.
     * @param spr Référence au sprite du joueur.
     * @param spd Vitesse horizontale du joueur.
     * @param ground Position du sol en Y.
     */
    Mouvement(sf::Sprite &sprite, float speed);


     /**
     * @brief Déplace le joueur vers la droite.
     */
    void moveRight();

    /**
     * @brief Déplace le joueur vers la gauche.
     */
    void moveLeft();

    /**
     * @brief Saut du joueur.
     */
    void jump();

    /**
     * @brief Vérifie si le joueur est en collision avec un bloc.
     * @param block Bloc avec lequel vérifier la collision.
     * @return Vrai si le joueur est en collision avec le bloc, faux sinon.
     */
    bool checkCollision(const sf::Sprite& block);

    /**
     * @brief Applique la gravité et gère le saut en l'air.
     * @param dx Permet de continuer à déplacer le joueur horizontalement pendant le saut.
     */
    void applyGravity(const std::vector<sf::Sprite>& blocks, const std::vector<sf::Sprite>& pipes);

    /**
     * @brief Vérifie si le joueur est toujours sur un bloc ou un tuyau.
     */
    bool blockMovement(const std::vector<sf::Sprite>& obstacles, const std::vector<sf::Sprite>& pipes);
    void checkForGaps(const std::vector<sf::FloatRect> &gaps);
};

#endif
