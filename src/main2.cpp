#include <SFML/Graphics.hpp>
#include "Headers/player.hpp"
#include <iostream>
#include "Headers/coin.hpp"
#include "Headers/background.hpp"
#include "Headers/camera.hpp"
#include "Headers/enemy.hpp"
#include "Headers/menu.hpp"
#include "Headers/friendlymushroom.hpp"
#include "Headers/game.hpp"


int main()
{

    // Menu window
    sf::RenderWindow menuWindow(sf::VideoMode(800, 600), "Mario Bros - Menu");
    Menu menu(800, 600);
    bool startGame = false;
    
    // Menu loop
    while (menuWindow.isOpen() && !startGame) {
        sf::Event event;
        while (menuWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                menuWindow.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    menu.moveUp();
                } else if (event.key.code == sf::Keyboard::Down) {
                    menu.moveDown();
                } else if (event.key.code == sf::Keyboard::Enter) {
                    int choice = menu.getSelectedIndex();
                    if (choice == 0) {
                        startGame = true;
                        menuWindow.close();
                    } else if (choice == 1) {
                        std::cout << "Options à implémenter..." << std::endl;
                    } else if (choice == 2) {
                        menuWindow.close();
                        return 0;
                    }
                }
            }
        }

        menuWindow.clear();
        menu.draw(menuWindow);
        menuWindow.display();
    }

    // Only start the game if menu option was selected
    if (!startGame) {
        return 0;
    }


    sf::RenderWindow window(sf::VideoMode(1280, 720), "Mario Bros");
    Background background;


    Game game;

   

    //Menu menu(800, 600);

    /**
     * Création des joueurs
     * Mario est contrôlé par les touches fléchées
     * Luigi est contrôlé par les touches M : Move et J : Jump
     */

    // Initialisation des joueurs

    Player mario("images/mario_resized.png", 100, 483, 0.07f, sf::Keyboard::Right, sf::Keyboard::Left, sf::Keyboard::Up);
    Player luigi("images/luigi.png", 200, 480, 0.07f, sf::Keyboard::M, sf::Keyboard::A, sf::Keyboard::J);

    // Création des différents types d'ennemis et leur zone de patrouille
    std::vector<std::unique_ptr<Enemy>> enemies;

    // // Goombas
    // enemies.push_back(std::make_unique<Goomba>(300, 545, 200, 400));
    // enemies.push_back(std::make_unique<Goomba>(500, 545, 450, 600));     

    // // Koopas
    enemies.push_back(std::make_unique<KoopaTroopa>(700, 545, 650, 850));
    // enemies.push_back(std::make_unique<KoopaTroopa>(900, 545, 800, 1000));

    // // Champignons gentils
    // enemies.push_back(std::make_unique<FriendlyMushroom>(400, 545, 350, 500));
    // enemies.push_back(std::make_unique<FriendlyMushroom>(800, 545, 750, 900));

    // Placement des pièces dans le niveau
    Coin coin1(300, 500);
    Coin coin2(400, 500);
    Coin coin3(500, 500);
    Coin coin4(600, 500);
    Coin coin5(700, 500);

    // Initialisation de la caméra qui suit les joueurs
    Camera camera(800, 600);

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

        // Calcul des limites de l'écran visible
        sf::Vector2f viewCenter = window.getView().getCenter();
        float viewWidth = window.getView().getSize().x;
        float screenLeft = viewCenter.x - (viewWidth / 2);
        float screenRight = viewCenter.x + (viewWidth / 2);

        // Obtenir les positions actuelles
        sf::Vector2f marioPos = mario.getPosition();
        sf::Vector2f luigiPos = luigi.getPosition();

        // Marge de sécurité depuis les bords de l'écran
        float margin = 80.0f;

        // Empêcher Mario de sortir de l'écran
        if (marioPos.x < screenLeft + margin) {
            mario.setPosition(screenLeft + margin, marioPos.y);
        }
        if (marioPos.x > screenRight - margin) {
            mario.setPosition(screenRight - margin, marioPos.y);
        }

        // Empêcher Luigi de sortir de l'écran
        if (luigiPos.x < screenLeft + margin) {
            luigi.setPosition(screenLeft + margin, luigiPos.y);
        }
        if (luigiPos.x > screenRight - margin) {
            luigi.setPosition(screenRight - margin, luigiPos.y);
        }

        // Calcul du point central entre les joueurs pour la caméra
        float camX = (marioPos.x + luigiPos.x) / 2.0f;
        float camY = 350.0f;

        // Déplacer la caméra seulement si les deux joueurs sont assez proches
        float playerDistance = std::abs(marioPos.x - luigiPos.x);
        if (playerDistance < viewWidth * 0.8f) {  // 80% de la largeur de l'écran
            camera.setCenter(camX, camY);
            window.setView(camera.getView());
        }

        for (auto& enemy : enemies)
        {
            if (enemy->isAlive())
            {
                enemy->interactWithPlayer(mario);
                enemy->interactWithPlayer(luigi);
            }
        }

        window.setView(camera.getView());

        if (!game.isGameOver()) {
            mario.update(background.getGroundTiles(), background.getPipes());
            luigi.update(background.getGroundTiles(), background.getPipes());
        
            mario.applyGravity(background.getGroundTiles(), background.getPipes());
            luigi.applyGravity(background.getGroundTiles(), background.getPipes());
        
            if (mario.checkWin(background.getFlag()) && !game.isGameOver()) {
                game.handleWin(1); // Mario gagne
            }
        
            if (luigi.checkWin(background.getFlag()) && !game.isGameOver()) {
                game.handleWin(2); // Luigi gagne
            }
        }
        


        // Affichage de tous les éléments du jeu
        window.clear();
        background.draw(window);
        mario.draw(window);
        luigi.draw(window);

        for (Coin *coin : coins)
        {
            coin->draw(window);
        }

        // Mise à jour de la physique et des collisions
        for (auto& enemy : enemies)
        {
            enemy->update();
            
            // Vérification si un joueur saute sur un ennemi
            sf::FloatRect players[] = {mario.getbounds(), luigi.getbounds()};

            for (const auto& playerBounds : players)
            {
                if (enemy->isAlive() && playerBounds.intersects(enemy->getBounds()))
                {
                    // Le joueur doit être au-dessus de l'ennemi pour l'écraser
                    if (playerBounds.top + playerBounds.height <= enemy->getBounds().top + 10)
                    {
                        enemy->onJumpedOn();
                        break;
                    }
                }
            }

            if (enemy->isAlive())
            {
                enemy->render(window);
            }
        }

        game.drawResult(window);

        window.display();
    }

    return 0;
}

