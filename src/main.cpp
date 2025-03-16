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
#include "Headers/fireflower.hpp"
#include "Headers/fireball.hpp"

int main()
{

    // Menu window
    sf::RenderWindow menuWindow(sf::VideoMode(800, 600), "Mario Bros - Menu");
    Menu menu(800, 600);
    bool startGame = false;

    // Menu loop
    while (menuWindow.isOpen() && !startGame)
    {
        sf::Event event;
        while (menuWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                menuWindow.close();

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Up)
                {
                    menu.moveUp();
                }
                else if (event.key.code == sf::Keyboard::Down)
                {
                    menu.moveDown();
                }
                else if (event.key.code == sf::Keyboard::Enter)
                {
                    int choice = menu.getSelectedIndex();
                    if (choice == 0)
                    {
                        startGame = true;
                        menuWindow.close();
                    }
                    else if (choice == 1)
                    {
                        menu.showRules();
                    }
                    else if (choice == 2)
                    {
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
    if (!startGame)
    {
        return 0;
    }

    sf::RenderWindow window(sf::VideoMode(1280, 720), "Mario Bros");
    Background background;

    Game game;

    FireFlower flower(1000, 500);

    // Menu menu(800, 600);

    /**
     * Création des joueurs
     * Mario est contrôlé par les touches fléchées
     * Luigi est contrôlé par les touches M : Move et J : Jump
     */

    // Initialisation des joueurs with character names
    Player mario("images/mario_resized.png", "Mario", 100, 483, 0.07f, 
                 sf::Keyboard::Right, sf::Keyboard::Left, sf::Keyboard::Up);
    Player luigi("images/luigi.png", "Luigi", 200, 480, 0.07f, 
                 sf::Keyboard::M, sf::Keyboard::A, sf::Keyboard::J);

    // Création des différents types d'ennemis et leur zone de patrouille
    std::vector<std::unique_ptr<Enemy>> enemies;

    // // Goombas
    enemies.push_back(std::make_unique<Goomba>(300, 545, 200, 400));
    // enemies.push_back(std::make_unique<Goomba>(500, 545, 450, 600));

    // // Koopas
    enemies.push_back(std::make_unique<KoopaTroopa>(700, 530, 650, 850));
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

        sf::Vector2f marioPos = mario.getPosition();
        sf::Vector2f luigiPos = luigi.getPosition();

        float margin = 80.0f;

        if (marioPos.x < screenLeft + margin)
        {
            mario.setPosition(screenLeft + margin, marioPos.y);
        }
        if (marioPos.x > screenRight - margin)
        {
            mario.setPosition(screenRight - margin, marioPos.y);
        }

        if (luigiPos.x < screenLeft + margin)
        {
            luigi.setPosition(screenLeft + margin, luigiPos.y);
        }
        if (luigiPos.x > screenRight - margin)
        {
            luigi.setPosition(screenRight - margin, luigiPos.y);
        }

        float camX = (marioPos.x + luigiPos.x) / 2.0f;
        float camY = 350.0f;

        float playerDistance = std::abs(marioPos.x - luigiPos.x);
        if (playerDistance < viewWidth * 0.8f)
        { 
            camera.setCenter(camX, camY);
            window.setView(camera.getView());
        }

        for (auto &enemy : enemies)
        {
            if (enemy->isAlive())
            {
                enemy->interactWithPlayer(mario);
                enemy->interactWithPlayer(luigi);
            }
        }

        window.setView(camera.getView());

        if (!game.isGameOver())
        {
            mario.update(background.getGroundTiles(), background.getPipes());
            luigi.update(background.getGroundTiles(), background.getPipes());

            mario.applyGravity(background.getGroundTiles(), background.getPipes());
            luigi.applyGravity(background.getGroundTiles(), background.getPipes());

            if (mario.checkWin(background.getFlag()) && !game.isGameOver())
            {
                game.handleWin(1); 
            }

            if (luigi.checkWin(background.getFlag()) && !game.isGameOver())
            {
                game.handleWin(2); 
            }
        }

        if (mario.getbounds().intersects(flower.getBounds()) && !flower.isCollected())
        {
            flower.collect();
            mario.collectFireFlower();
            std::cout << "Mario a collecté une fleur de feu !" << std::endl;
        }

        if (luigi.getbounds().intersects(flower.getBounds()) && !flower.isCollected())
        {
            flower.collect();
            luigi.collectFireFlower();
            std::cout << "Luigi a collecté une fleur de feu !" << std::endl;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            mario.shootFireball();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
            std::cout << "F key pressed - Luigi attempting to shoot" << std::endl;
            luigi.shootFireball();
        }

        mario.updateFireballs(background.getGroundTiles());
        luigi.updateFireballs(background.getGroundTiles());

        auto checkFireballCollisions = [&](Player& player, const std::string& playerName) {
            auto& fireballs = player.getFireballs();
            for (auto& fireball : fireballs) {
                if (fireball.isActive()) {
                    for (auto& enemy : enemies) {
                        if (enemy->isAlive() && fireball.getBounds().intersects(enemy->getBounds())) {
                            enemy->onFireballHit(); 
                            fireball.destroy();
                            std::cout << playerName << " a tué un ennemi avec une boule de feu!" << std::endl;
                            break;
                        }
                    }
                }
            }
        };
        
        checkFireballCollisions(mario, "Mario");
        checkFireballCollisions(luigi, "Luigi");

        window.clear();
        background.draw(window);
        mario.draw(window);
        luigi.draw(window);
        mario.drawFireballs(window);
        luigi.drawFireballs(window);

        flower.draw(window);
        for (Coin *coin : coins)
        {
            coin->draw(window);
        }

        for (auto &enemy : enemies)
        {
            if (enemy->isAlive())
            {
                enemy->checkForGaps(background.getGaps());
                enemy->update();
                
         
                sf::FloatRect marioRect = mario.getbounds();
                sf::FloatRect luigiRect = luigi.getbounds();
                sf::FloatRect enemyRect = enemy->getBounds();
                
              
                if (marioRect.intersects(enemyRect) && 
                    marioRect.top + marioRect.height <= enemyRect.top + 10)
                {
                    enemy->onJumpedOn();
                    mario.bounce();
                    std::cout << "Mario a sauté sur l'ennemi!" << std::endl;
                }
                else if (luigiRect.intersects(enemyRect) && 
                         luigiRect.top + luigiRect.height <= enemyRect.top + 10)
                {
                    enemy->onJumpedOn();
                    luigi.bounce();
                    std::cout << "Luigi a sauté sur l'ennemi!" << std::endl;
                }
            
                else if (enemy->isAlive())
                {
                    enemy->interactWithPlayer(mario);
                    enemy->interactWithPlayer(luigi);
                }
                
             
                if (enemy->isAlive())
                {
                    enemy->render(window);
                }
            }
        }

        game.drawResult(window);

        window.display();
    }

    return 0;
}
