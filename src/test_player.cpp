#include <SFML/Graphics.hpp>
#include "Headers/player.hpp"
#include "Headers/camera.hpp"
#include "Headers/game.hpp"

int main() {
    /**
     * Création de la fenêtre
     */
    sf::RenderWindow window(sf::VideoMode(800, 600), "Test Multiplayer");

    /**
     * Création de delta
     * Il permet de normaliser la vitesse d'exécution du programme 
     * afin qu'elle reste constante sur des ordinateurs de puissances différentes.
     */
    sf::Clock deltaClock; 

    /**
     * Création des joueurs
     * Mario est contrôlé par les touches fléchées
     * Luigi est contrôlé par les touches M : Move et J : Jump
     */
    Player mario("../images/mario_resized.png", 400, 300, 3.0f, sf::Keyboard::Right, sf::Keyboard::Up);
    Player luigi("../images/luigi.png", 400, 300, 5.0f, sf::Keyboard::M, sf::Keyboard::J);


    //Camera camera;
    Camera camera(800, 600);


    begin(window);
    /**
     * Boucle principale
     * Gestion des événements, mise à jour et affichage des joueurs
     */
    while (window.isOpen()) {
        float deltaTime = deltaClock.restart().asSeconds(); //Met à jour deltaTime avec le temps écoulé (en secondes) depuis le dernier appel à restart()
        sf::Event event;
        while (window.pollEvent(event)) { //récupère les évènements de la fenêtre (souris, clavier etc)
            if (event.type == sf::Event::Closed) //pour fermer la fenêtre avec un clic sur la croix de la fenêtre
                window.close();

            else if (event.type == sf::Event::Resized)
            camera.update({event.size.width, event.size.height});
        }

        window.setView(camera.getView());
        //update(deltaTime);



        mario.update();
        luigi.update();
        mario.jump();
        luigi.jump();
        mario.applyGravity();
        luigi.applyGravity();

        window.clear(sf::Color::White); //actualise l'écran


        //render(window); //dessin

        mario.draw(window);
        luigi.draw(window);

        window.display();
    }

    return 0;
}

