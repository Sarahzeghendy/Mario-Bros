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
    sf::RenderWindow window(sf::VideoMode(800, 600), "Mario Bros - Menu");

    
    Menu menu(800, 600);
    
    Camera camera(800, 600);
    /**
     * Boucle principale du menu
     * Gestion des événements, mise à jour et affichage du menu
     * Gestion de la sélection des options du menu
     */
    bool startGame = false;
    
    while (window.isOpen() && !startGame) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    menu.moveUp();
                } else if (event.key.code == sf::Keyboard::Down) {
                    menu.moveDown();
                } else if (event.key.code == sf::Keyboard::Enter) {
                    int choice = menu.getSelectedIndex();
                    if (choice == 0) { 
                        startGame = true;  // ✅ Lancer le jeu
                        window.close();  // 🚀 Fermer la fenêtre du menu
                    } else if (choice == 1) {
                        std::cout << "Options à implémenter..." << std::endl;
                    } else if (choice == 2) {
                        window.close();  // 🚀 Quitter le jeu
                    }
                }
            }
        }

        window.clear();
        menu.draw(window);
        window.display();
    }

    /**
     * Lancer le jeu
     * Création de la fenêtre de jeu, des joueurs, des ennemis et des pièces
     */
    if (startGame) {
        sf::RenderWindow gameWindow(sf::VideoMode(800, 600), "Mario Bros - Jeu");

        Background background;

        const std::vector<sf::Sprite>& bricks = background.getBricks();
        const std::vector<sf::Sprite>& pipes = background.getPipes();

        std::vector<sf::Sprite> obstacles;
        obstacles.insert(obstacles.end(), bricks.begin(), bricks.end());
        obstacles.insert(obstacles.end(), pipes.begin(), pipes.end());
        
        Player mario("images/mario_resized.png", 100, 500, 0.07f, sf::Keyboard::Right, sf::Keyboard::Left, sf::Keyboard::Up);
        Player luigi("images/luigi.png", 200, 500, 0.07f, sf::Keyboard::M, sf::Keyboard::D, sf::Keyboard::J);

        /**
     * Création des ennemis
     */
        Enemy mushroom1("images/mushroom.png", 700, 520, 0, 1000);//position 100 en horizontale et 515 en verticale
        Enemy mushroom2("images/mushroom.png", 450, 520,0, 1000);
        Enemy mushroom3("images/mushroom.png", 600, 520,0,1000);

    /**
     * Création des gentils champignons
     */

        FriendlyMushroom bonus("images/champignon.png", 700, 520, 0, 1000);//position 100 en horizontale et 515 en verticale

    /**
     * Création des pièces individuellement (sans `for`)
     */
        Coin coin1(300, 500);
        Coin coin2(400, 500);
        Coin coin3(500, 500);
        Coin coin4(600, 500);
        Coin coin5(700, 500);

        Camera camera(800, 600);

        while (gameWindow.isOpen())
        {
            sf::Event event;
            while (gameWindow.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    gameWindow.close();
                else if (event.type == sf::Event::Resized)
                    camera.update({event.size.width, event.size.height});
            }

            Coin* coins[] = {&coin1, &coin2, &coin3, &coin4, &coin5};
            for (Coin* coin : coins)
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

            gameWindow.setView(camera.getView());
            // Calculate camera boundaries and maximum separation distance
            float maxPlayerSeparation = gameWindow.getSize().x * 0.8f; // 80% of screen width
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
        gameWindow.setView(camera.getView());
        mario.update(obstacles);
        luigi.update(obstacles);
        mario.jump();
        luigi.jump();
        mario.applyGravity(bricks, pipes);
        luigi.applyGravity(bricks, pipes);

        // Effacer l'écran
        gameWindow.clear();
        //menu.draw(window);
        background.draw(gameWindow);
        mario.draw(gameWindow);
        luigi.draw(gameWindow);
        //mushroom1.draw(window);

        /**
         * Dessiner chaque pièce individuellement
         */
        for (Coin *coin : coins)
        {
            coin->draw(gameWindow);
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
            enemy->render(gameWindow);
        }
    }
        // Dessin du champignon gentil
        bonus.render(gameWindow);

        gameWindow.display();
    }

    }
    

    return 0;
}
