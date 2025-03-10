#ifndef ENEMY_HPP
#define ENEMY_HPP

class Player; //déclaration aticipée de la classe Player

#include <SFML/Graphics.hpp>
#include "player.hpp"
#include "mouvement.hpp"


class Enemy 
{
    public:
        Enemy(const std::string& imagePath,float x, float y, float leftLim, float rightLim);  // constructeur
        void update();            // Met à jour l'ennemi
        void render(sf::RenderWindow& window);  // Affiche l'ennemi
        void onJumpedOn();        // Appelé quand un joueur saute dessus
        bool isAlive() const;     // Vérifie si l'ennemi est encore en vie
        virtual void interactWithPlayer(Player& player);  // Interaction avec le joueur

        sf::FloatRect getBounds() const; // Retourne la hitbox de l'ennemi (dimensions et la position de la zone où l'ennemi peut être touché ou interagir avec d'autres objets)
        void draw(sf::RenderWindow& window);

        sf::Texture texture;
        sf::Sprite sprite;

    private:
        
        
        bool alive;

        Mouvement mouvement;           // L'objet Mouvement pour gérer le déplacement
        bool movingRight;              // Direction du mouvement (vers la droite ou la gauche)
        float leftLimit, rightLimit;   // Limites de déplacement

};

#endif
