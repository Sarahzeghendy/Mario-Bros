#include <SFML/Graphics.hpp>
#include "Headers/player.hpp"
#include <iostream>
#include "Headers/coin.hpp"
#include "Headers/background.hpp"
#include "Headers/camera.hpp"
#include "Headers/enemy.hpp"
#include "Headers/menu.hpp"
#include "Headers/friendlymushroom.hpp"

int main()
{
    /**
     * Création de la fenêtre
     */
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Mario Bros");

    // Create background
    Background background;

   

   

    //Menu menu(800, 600);

    /**
     * Création des joueurs
     * Mario est contrôlé par les touches fléchées
     * Luigi est contrôlé par les touches M : Move et J : Jump
     */
    Player mario("images/mario_resized.png", 100, 483, 0.07f, sf::Keyboard::Right, sf::Keyboard::Left, sf::Keyboard::Up);
    Player luigi("images/luigi.png", 200, 480, 0.07f, sf::Keyboard::M, sf::Keyboard::A, sf::Keyboard::J);

    /**
     * Création des ennemis
     */
    Enemy mushroom1("images/mushroom.png", 700, 545, 0, 1000);//position 100 en horizontale et 515 en verticale
    Enemy mushroom2("images/mushroom.png", 450, 545,0, 1000);
    Enemy mushroom3("images/mushroom.png", 600, 545,0,1000);

    /**
     * Création des gentils champignons
     */

    FriendlyMushroom bonus("images/champignon.png", 700, 545, 0, 1000);//position 100 en horizontale et 515 en verticale

    /**
     * Création des pièces individuellement (sans `for`)
     */
    Coin coin1(300, 500);
    Coin coin2(400, 500);
    Coin coin3(500, 500);
    Coin coin4(600, 500);
    Coin coin5(700, 500);

    // Camera
    Camera camera(800, 600);

    /**
     * Boucle principale
     * Gestion des événements, mise à jour et affichage des joueurs et des pièces
     */
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::Resized)
                camera.update({event.size.width, event.size.height});
        }

        // Menu
        // if (event.type == sf::Event::KeyPressed) {
        //     if (event.key.code == sf::Keyboard::Up) {
        //         menu.moveUp();
        //     } else if (event.key.code == sf::Keyboard::Down) {
        //         menu.moveDown();
        //     } else if (event.key.code == sf::Keyboard::Enter) {
        //         int choice = menu.getSelectedIndex();
        //         if (choice == 0) {
        //             // Lancer le jeu
        //             std::cout << "Lancement du jeu !" << std::endl;
        //         } else if (choice == 1) {
        //             std::cout << "Options à implémenter..." << std::endl;
        //         } else if (choice == 2) {
        //             window.close();
        //         }
        //     }
        // }

        /**
         * Vérifier si Mario ou Luigi touche une pièce
         */
        Coin *coins[] = {&coin1, &coin2, &coin3, &coin4, &coin5};
        for (Coin *coin : coins)
        {
            if (!coin->isCollected())
            {
                if (mario.getbounds().intersects(coin->getBounds()))
                {
                    coin->collect();
                    mario.getCoins();
                    std::cout << "Mario a collecté une pièce !" << std::endl;
                }
                if (luigi.getbounds().intersects(coin->getBounds()))
                {
                    coin->collect();
                    luigi.getCoins();
                    std::cout << "Luigi a collecté une pièce !" << std::endl;
                }
            }
        }

        // Calculate the average position of both players
        sf::Vector2f marioPos = mario.getPosition();
        sf::Vector2f luigiPos = luigi.getPosition();
        sf::Vector2f marioCurrentPos = marioPos;  // Add this line
        sf::Vector2f luigiCurrentPos = luigiPos;  // Add this line

        // Calcul de la position de la camera en fonction de la position moyenne des joueurs
        float camX = (marioPos.x + luigiPos.x) / 2.0f;
        float camY = 350.0f; // Position Y fixe

        // Update camera position based on average player position
        camera.setCenter(camX, camY);

        window.setView(camera.getView());

        // Calculate camera boundaries and maximum separation distance
        float maxPlayerSeparation = window.getSize().x * 0.8f; // 80% of screen width
        float currentSeparation = std::abs(marioCurrentPos.x - luigiCurrentPos.x);

        // Check if players are too far apart
        if (currentSeparation > maxPlayerSeparation) {
            // If Mario is on the right
            if (marioCurrentPos.x > luigiCurrentPos.x) {
                if (mario.isMovingRight()) {
                    mario.setPosition(luigiCurrentPos.x + maxPlayerSeparation, marioCurrentPos.y);
                }
                if (luigi.isMovingLeft()) {
                    luigi.setPosition(marioCurrentPos.x - maxPlayerSeparation, luigiCurrentPos.y);
                }
            }
            // If Luigi is on the right
            else {
                if (luigi.isMovingRight()) {
                    luigi.setPosition(marioCurrentPos.x + maxPlayerSeparation, luigiCurrentPos.y);
                }
                if (mario.isMovingLeft()) {
                    mario.setPosition(luigiCurrentPos.x - maxPlayerSeparation, marioCurrentPos.y);
                }
            }
        }

        /**
         * Vérifier si Mario ou Luigi touche un ennemi
         */

        // Enemy* enemies[] = {&mushroom1, &mushroom2, &mushroom3};
        // for (Enemy* enemy : enemies)
        // {
        //     if (enemy->isAlive()) // Vérifie si l'ennemi est encore vivant
        //     {
        //         mario.checkCollision(*enemy);
        //         luigi.checkCollision(*enemy);
        //     }
        // }

        /**
         * Vérifier si Mario ou Luigi touche un ennemi
         */
        Enemy* enemies[] = {&mushroom1, &mushroom2, &mushroom3};
        for (Enemy* enemy : enemies)
        {
            if (enemy->isAlive()) // Vérifie si l'ennemi est encore vivant
            {
                
                enemy->interactWithPlayer(mario);  // Passe l'objet mario
                enemy->interactWithPlayer(luigi);  // Passe l'objet luigi
            }
        }

        /**
         * Interaction avec le champignon gentil
         */
        bonus.update(); // Ajout de la mise à jour du champignon gentil
        bonus.interactWithPlayer(mario); 
        bonus.interactWithPlayer(luigi);


        // Mise à jour des joueurs
        window.setView(camera.getView());
        mario.update(background.getGroundTiles(), background.getPipes());
        luigi.update(background.getGroundTiles(), background.getPipes());
        mario.applyGravity(background.getGroundTiles(), background.getPipes());
        luigi.applyGravity(background.getGroundTiles(), background.getPipes());
      

        // Effacer l'écran
        window.clear();
        //menu.draw(window);
        background.draw(window);
        mario.draw(window);
        luigi.draw(window);
        //mushroom1.draw(window);

        /**
         * Dessiner chaque pièce individuellement
         */
        for (Coin *coin : coins)
        {
            coin->draw(window);
        }

    /**
     * Dessiner les ennemis
     */
    //Enemy* enemies[] = {&mushroom1, &mushroom2, &mushroom3};
    for (Enemy* enemy : enemies)
    {
        enemy->update();

        // Tableau des joueurs (Mario et Luigi)
        sf::FloatRect players[] = {mario.getbounds(), luigi.getbounds()};

        // Vérifier si l'un des deux joueurs écrase l'ennemi
        for (const auto& playerBounds : players)
        {
            if (enemy->isAlive() && playerBounds.intersects(enemy->getBounds()))
            {
                // Vérifier si le joueur atterrit sur le dessus de l'ennemi
                if (playerBounds.top + playerBounds.height <= enemy->getBounds().top + 10) //décalage de 10 unités 
                {
                    enemy->onJumpedOn();
                    break; // Pas besoin de vérifier l'autre joueur si l'ennemi est déjà écrasé
                }
            }
        }


        // Dessiner l'ennemi seulement s'il est encore en vie
        if (enemy->isAlive())
        {
            enemy->render(window);
        }
    }
        // Dessin du champignon gentil
        bonus.render(window);

        window.display();
    }


    

    return 0;
}
