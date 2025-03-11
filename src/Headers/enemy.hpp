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
        virtual ~Enemy() = default;

        virtual void update() = 0;
        virtual void interactWithPlayer(Player& player) = 0;
        virtual void onJumpedOn() = 0;
        
        // Fonctions communes à tous les ennemis
        void render(sf::RenderWindow& window);  // Affiche l'ennemi
        bool isAlive() const;     // Vérifie si l'ennemi est encore en vie
        sf::FloatRect getBounds() const; // Retourne la hitbox de l'ennemi (dimensions et la position de la zone où l'ennemi peut être touché ou interagir avec d'autres objets)
        void draw(sf::RenderWindow& window);

    protected:
        sf::Texture texture;
        sf::Sprite sprite;
        bool alive;
        Mouvement mouvement;           // L'objet Mouvement pour gérer le déplacement
        bool movingRight;              // Direction du mouvement
        float leftLimit, rightLimit;   // Limites de déplacement

};


// Les classes Goomba et KoopaTroopa héritent de la classe Enemy
class Goomba : public Enemy {
    public:
        Goomba(float x, float y, float leftLim, float rightLim);
        void update() override;
        void interactWithPlayer(Player& player) override;
        void onJumpedOn() override;
};

class KoopaTroopa : public Enemy {
    public:
        KoopaTroopa(float x, float y, float leftLim, float rightLim);
        void update() override;
        void interactWithPlayer(Player& player) override;
        void onJumpedOn() override;
    
    private:
        sf::Texture koopaShell;  // Texture pour la carapace du Koopa
};

#endif
