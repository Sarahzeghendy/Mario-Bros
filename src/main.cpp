#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Headers/player.hpp"
#include "Headers/coin.hpp"
#include "Headers/background.hpp"
#include "Headers/camera.hpp"
#include "Headers/enemy.hpp"
#include "Headers/menu.hpp"
#include "Headers/game.hpp"
#include "Headers/fireflower.hpp"
#include "Headers/fireball.hpp"
#include "Headers/AIPlayer.hpp"
#include "Headers/etoile.hpp"
#include "Headers/score.hpp"

/**
 * @brief Fonction principale du jeu
 * @return Code de retour du programme (0 si tout s'est bien passe)
 */
int main()
{
    // Variable pour controler la boucle de redemarrage du jeu
    bool restartGame = true;

    // Boucle principale permettant de recommencer le jeu
    while (restartGame) 
    {
        restartGame = false;

        /**
         * Configuration de la musique
         */
        sf::Music music;
        if (!music.openFromFile("musiques/mario.wav")) 
        {
            std::cerr << "Erreur : impossible de charger la musique !\n";
            return -1;
        }
        music.setLoop(true);
        music.play();

        /**
         * Creation et configuration du menu
         */
        sf::RenderWindow menuWindow(sf::VideoMode(800, 600), "Mario Bros - Menu");
        Menu menu(800, 600);
        bool startGame = false;
        bool isAIMode = false;

        // Boucle d'affichage et gestion du menu
        while (menuWindow.isOpen() && !startGame)
        {
            sf::Event event;
            while (menuWindow.pollEvent(event))
            {
                // Gestion de la fermeture de la fenetre
                if (event.type == sf::Event::Closed)
                    menuWindow.close();

                // Gestion des touches de navigation dans le menu
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
                        if (choice == 0) // Mode Joueur Solo
                        {
                            startGame = true;
                            isAIMode = false;
                            menuWindow.close();
                        }
                        else if (choice == 1) // Mode Joueur contre IA
                        {
                            startGame = true;
                            isAIMode = true;
                            menuWindow.close();
                        }
                        else if (choice == 2) // Regles du jeu
                        {
                            menu.showRules();
                        }
                        else if (choice == 3) // Quitter
                        {
                            menuWindow.close();
                            return 0;
                        }
                    }
                }
            }
            
            // Dessin du menu
            menuWindow.clear();
            menu.draw(menuWindow);
            menuWindow.display();
        }
        
        // Verification si le jeu doit demarrer
        if (!startGame)
        {
            return 0;
        }

        /**
         * Configuration de la fenetre principale du jeu
         */
        sf::RenderWindow gameWindow(sf::VideoMode(1280, 720),
                                    isAIMode ? "Mario Bros - Joueur contre IA" : "Mario Bros");

        /**
         * Creation des elements du jeu
         */
        Background background;
        Game game;

        // Creation des power-ups: fleurs de feu
        FireFlower flower(900, 370);
        FireFlower flower1(1940, 370);
        FireFlower flower2(6550, 370);
        FireFlower *flowers[] = {&flower, &flower1, &flower2};

        // Creation des power-ups: etoiles
        Etoile etoile(520, 410);
        Etoile etoile2(3185, 410);
        Etoile etoile3(6235, 410);
        Etoile etoile4(6870, 410);
        Etoile etoile5(1190, 410);
        Etoile etoile6(8485, 410);
        Etoile *etoiles[] = {&etoile, &etoile2, &etoile5, &etoile3, &etoile4, &etoile6};

        // Creation des joueurs avec leurs controles respectifs
        Player mario("images/sprite.jpg", "Mario", 100, 483, 0.7f,
                     sf::Keyboard::Right, sf::Keyboard::Left, sf::Keyboard::Up);
        Player luigi("images/sprite.jpg", "Luigi", 150, 480, 0.7f,
                     sf::Keyboard::D, sf::Keyboard::Q, sf::Keyboard::Z);

        // Definition des rectangles de texture pour les sprites des joueurs
        mario.getSprite().setTextureRect(sf::IntRect(8, 139, 28, 47));
        luigi.getSprite().setTextureRect(sf::IntRect(8, 191, 28, 47));

        /**
         * Creation des ennemis
         */
        std::vector<std::unique_ptr<Enemy>> enemies;
        // Ajout des ennemis de differents types a differentes positions
        enemies.push_back(std::make_unique<Goomba>(700, 545));
        enemies.push_back(std::make_unique<KoopaTroopa>(750, 530));
        enemies.push_back(std::make_unique<Goomba>(1000, 545));
        enemies.push_back(std::make_unique<KoopaTroopa>(1300, 530));
        enemies.push_back(std::make_unique<FriendlyMushroom>(1500, 540));
        enemies.push_back(std::make_unique<Goomba>(3250, 545));
        enemies.push_back(std::make_unique<KoopaTroopa>(3350, 530));
        enemies.push_back(std::make_unique<FriendlyMushroom>(3300, 540));
        enemies.push_back(std::make_unique<Goomba>(3200, 545));
        enemies.push_back(std::make_unique<KoopaTroopa>(3190, 530));
        enemies.push_back(std::make_unique<Goomba>(4550, 545));
        enemies.push_back(std::make_unique<KoopaTroopa>(4600, 530));
        enemies.push_back(std::make_unique<FriendlyMushroom>(4700, 540));
        enemies.push_back(std::make_unique<Goomba>(5100, 545));
        enemies.push_back(std::make_unique<KoopaTroopa>(5200, 530));
        enemies.push_back(std::make_unique<FriendlyMushroom>(5300, 540));
        enemies.push_back(std::make_unique<Goomba>(7500, 545));
        enemies.push_back(std::make_unique<KoopaTroopa>(7600, 530));
        enemies.push_back(std::make_unique<FriendlyMushroom>(7700, 540));
        enemies.push_back(std::make_unique<Goomba>(8500, 545));
        enemies.push_back(std::make_unique<KoopaTroopa>(8600, 530));
        enemies.push_back(std::make_unique<FriendlyMushroom>(8700, 540));

        /**
         * Creation des pieces a collecter
         */
        Coin coin1(400, 500);
        Coin coin2(420, 500);
        Coin coin3(500, 300);
        Coin coin4(550, 300);
        Coin coin5(600, 300);
        Coin coin6(1300, 400);
        Coin coin7(1340, 400);
        Coin coin8(1380, 400);
        Coin coin9(1420, 400);
        Coin coin10(1460, 400);
        Coin coin11(3000, 400);
        Coin coin12(3040, 400);
        Coin coin13(3080, 400);
        Coin coin14(1620, 400);
        Coin coin15(8760, 400);
        Coin coin16(8800, 400);
        Coin coin17(8840, 400);
        Coin coin18(5780, 400);
        Coin coin19(5820, 400);
        Coin coin20(6860, 400);
        Coin coin21(6900, 400);
        Coin coin22(6940, 400);
        Coin *coins[] = {&coin1, &coin2, &coin3, &coin4, &coin5, &coin6, &coin7, &coin8, &coin9, &coin10, &coin11, &coin12, &coin13, &coin14, &coin15, &coin16, &coin17, &coin18, &coin19, &coin20, &coin21, &coin22};

        /**
         * Configuration du mode IA si active
         */
        AIPlayer *aiController = nullptr;
        if (isAIMode)
        {
            aiController = new AIPlayer(&luigi, enemies);
        }

        /**
         * Configuration de la camera
         */
        Camera camera(800, 600);

        /**
         * Chargement des polices et configuration des scores
         */
        sf::Font font;
        if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) 
        {
            std::cout << "Error loading font" << std::endl;
            return -1;
        }

        Score marioScore("images/mario_score.png", font);
        Score luigiScore("images/luigi_score.png", font);

        /**
         * Configuration du systeme de reapparition des joueurs
         */
        const int respawnDelay = 120; 
        int marioRespawnTimer = 0;
        int luigiRespawnTimer = 0;
        
        sf::Text marioRespawnText;
        sf::Text luigiRespawnText;
        marioRespawnText.setFont(font);
        luigiRespawnText.setFont(font);
        marioRespawnText.setCharacterSize(20);
        luigiRespawnText.setCharacterSize(20);
        marioRespawnText.setFillColor(sf::Color::White);
        luigiRespawnText.setFillColor(sf::Color::White);
        marioRespawnText.setString("Mario respawning...");
        luigiRespawnText.setString("Luigi respawning...");

        /**
         * Boucle principale du jeu
         */
        while (gameWindow.isOpen())
        {
            /**
             * Gestion des evenements
             */
            sf::Event event;
            while (gameWindow.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    gameWindow.close();
                else if (event.type == sf::Event::Resized)
                    camera.update({event.size.width, event.size.height});
            }

            /**
             * Gestion des collisions entre joueurs
             */
            if (mario.getbounds().intersects(luigi.getbounds())) 
            {
                // Si Mario est au-dessus de Luigi
                if (mario.getPosition().y + mario.getbounds().height <= luigi.getPosition().y + 5) 
                {
                    mario.getMouvement().jump();
                } 
                // Si Luigi est au-dessus de Mario
                else if (luigi.getPosition().y + luigi.getbounds().height <= mario.getPosition().y + 5) 
                {
                    luigi.getMouvement().jump();
                }
                else // Collision laterale
                {
                    bool marioBouge = mario.isMoving();
                    bool luigiBouge = luigi.isMoving();
                    
                    // Empechement de traversee horizontale
                    if (luigiBouge) 
                    {
                        if (luigi.getPosition().x < mario.getPosition().x) 
                        {
                            luigi.setPosition(mario.getPosition().x - luigi.getbounds().width, luigi.getPosition().y);
                        } 
                        else if (luigi.getPosition().x > mario.getPosition().x) 
                        {
                            luigi.setPosition(mario.getPosition().x + mario.getbounds().width, luigi.getPosition().y);
                        }
                    }

                    if (marioBouge) 
                    {
                        if (mario.getPosition().x < luigi.getPosition().x) 
                        {
                            mario.setPosition(luigi.getPosition().x - mario.getbounds().width, mario.getPosition().y);
                        } 
                        else if (mario.getPosition().x > luigi.getPosition().x) 
                        {
                            mario.setPosition(luigi.getPosition().x + luigi.getbounds().width, mario.getPosition().y);
                        }
                    }
                }
            }

            /**
             * Gestion des chutes des joueurs hors de l'ecran
             */
            float lowerLimit = gameWindow.getSize().y + 50; 
            if (mario.getPosition().y > lowerLimit && !mario.getIsDead())
            {
                mario.loseLife();
                if (mario.getLives() <= 0)
                {
                    game.handleWin(2); 
                }
                else
                {
                    marioRespawnTimer = respawnDelay;
                }
            }

            if (luigi.getPosition().y > lowerLimit && !luigi.getIsDead())
            {
                luigi.loseLife();
                if (luigi.getLives() <= 0)
                {
                    game.handleWin(1); 
                }
                else
                {
                    luigiRespawnTimer = respawnDelay;
                }
            }

            /**
             * Gestion de la reapparition des joueurs
             */
            if (mario.shouldRespawn() && marioRespawnTimer > 0)
            {
                marioRespawnTimer--;
                marioRespawnText.setString("Mario respawning in " + std::to_string((marioRespawnTimer / 60) + 1) + "...");
                marioRespawnText.setPosition(mario.getPosition().x - 50, mario.getPosition().y - 40);

                if (marioRespawnTimer <= 0)
                {
                    mario.respawn();
                }
            }

            if (luigi.shouldRespawn() && luigiRespawnTimer > 0)
            {
                luigiRespawnTimer--;

                luigiRespawnText.setString("Luigi respawning in " + std::to_string((luigiRespawnTimer / 60) + 1) + "...");
                luigiRespawnText.setPosition(luigi.getPosition().x - 50, luigi.getPosition().y - 40);

                if (luigiRespawnTimer <= 0)
                {
                    luigi.respawn();
                }
            }

            /**
             * Gestion de l'ecran de fin de partie
             */
            if (game.isGameOver())
            {
                // Configuration du bouton de retour au menu
                sf::Text returnToMenuText;
                returnToMenuText.setFont(font);
                returnToMenuText.setString("Return to Menu");
                returnToMenuText.setCharacterSize(30);
                returnToMenuText.setFillColor(sf::Color::White);

                sf::RectangleShape menuButton;
                menuButton.setSize(sf::Vector2f(300, 60));
                menuButton.setFillColor(sf::Color(50, 50, 200, 230)); 
                menuButton.setOutlineThickness(3);
                menuButton.setOutlineColor(sf::Color::White);

                sf::Vector2f viewCenter = gameWindow.getView().getCenter();

                menuButton.setOrigin(menuButton.getSize().x / 2, menuButton.getSize().y / 2);
                menuButton.setPosition(viewCenter.x, viewCenter.y + 80);

                sf::FloatRect textBounds = returnToMenuText.getLocalBounds();
                returnToMenuText.setOrigin(textBounds.width / 2, textBounds.height / 2);
                returnToMenuText.setPosition(viewCenter.x, viewCenter.y + 80);

                // Boucle d'affichage de l'ecran de fin
                bool buttonHovered = false;
                while (gameWindow.isOpen())
                {
                    sf::Event event;
                    while (gameWindow.pollEvent(event))
                    {
                        if (event.type == sf::Event::Closed)
                        {
                            gameWindow.close();
                            return 0;
                        }

                        sf::Vector2i mousePos = sf::Mouse::getPosition(gameWindow);
                        sf::Vector2f worldPos = gameWindow.mapPixelToCoords(mousePos);

                        if (menuButton.getGlobalBounds().contains(worldPos))
                        {
                            if (!buttonHovered)
                            {
                                menuButton.setFillColor(sf::Color(100, 100, 255, 230)); 
                                menuButton.setOutlineColor(sf::Color::Yellow);
                                returnToMenuText.setFillColor(sf::Color::Yellow);
                                buttonHovered = true;
                            }

                            if (event.type == sf::Event::MouseButtonPressed &&
                                event.mouseButton.button == sf::Mouse::Left)
                            {
                                std::cout << "Return to menu button clicked!" << std::endl;
                                gameWindow.close(); 
                                restartGame = true; 
                                break;
                            }
                        }
                        else if (buttonHovered)
                        {
                            menuButton.setFillColor(sf::Color(50, 50, 200, 230)); 
                            menuButton.setOutlineColor(sf::Color::White);
                            returnToMenuText.setFillColor(sf::Color::White);
                            buttonHovered = false;
                        }
                    }

                    if (restartGame)
                    {
                        break; 
                    }

                    // Dessiner le fond
                    gameWindow.clear();
                    background.draw(gameWindow);
                    game.drawResult(gameWindow);

                    gameWindow.draw(menuButton);
                    gameWindow.draw(returnToMenuText);

                    // Ajouter les instructions
                    sf::Text instructionText;
                    instructionText.setFont(font);
                    instructionText.setString("Click the button to return to menu");
                    instructionText.setCharacterSize(20);
                    instructionText.setFillColor(sf::Color::White);
                    instructionText.setPosition(viewCenter.x - 150, viewCenter.y + 130);
                    gameWindow.draw(instructionText);

                    gameWindow.display();
                }

                if (restartGame)
                {
                    break; 
                }
            }

            /**
             * Mise a jour du controleur IA si actif
             */
            if (isAIMode)
            {
                aiController->update(background.getGroundTiles(), background.getPipes(), background.getFlag());
                
                // Gestion des entrees clavier pour Mario
                mario.setMovingRight(sf::Keyboard::isKeyPressed(sf::Keyboard::Right));
                mario.setMovingLeft(sf::Keyboard::isKeyPressed(sf::Keyboard::Left));

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !mario.isInAir())
                {
                    mario.jump();
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && mario.hasFirePowerActive())
                {
                    mario.shootFireball();
                }
            }
   
            /**
             * Gestion de la collecte des pieces
             */
            for (Coin *coin : coins)
            {
                // Verification et traitement des collisions avec les pieces
                if (!coin->isCollected())
                {
                    if (mario.getbounds().intersects(coin->getBounds()))
                    {
                        coin->collect();
                        mario.getCoins();
                    }
                    if (luigi.getbounds().intersects(coin->getBounds()))
                    {
                        coin->collect();
                        luigi.getCoins();
                    }
                }
            }

            /**
             * Gestion de la camera et des limites de l'ecran
             */
            sf::Vector2f viewCenter = gameWindow.getView().getCenter();
            float viewWidth = gameWindow.getView().getSize().x;
            float screenLeft = viewCenter.x - (viewWidth / 2);
            float screenRight = viewCenter.x + (viewWidth / 2);

            // Maintien des joueurs dans les limites visibles
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

            // Centrage de la camera entre les joueurs
            float camX = (mario.getPosition().x + luigi.getPosition().x) / 2.0f;
            float camY = 350.0f;

            float playerDistance = std::abs(mario.getPosition().x - luigi.getPosition().x);
            if (playerDistance < viewWidth * 0.8f)
            {
                camera.setCenter(camX, camY);
                gameWindow.setView(camera.getView());
            }

            /**
             * Gestion des interactions joueur-ennemi
             */
            for (auto &enemy : enemies)
            {
                // Traitement des interactions entre ennemis et joueurs
                if (enemy->isAlive())
                {
                    if (!mario.getIsDead())
                    {
                        bool wasAlive = true; 
                        enemy->interactWithPlayer(mario);

                        if (mario.getIsDead() && wasAlive)
                        {
                            if (mario.getLives() > 0)
                            {
                                marioRespawnTimer = respawnDelay;
                            }
                            else
                            {
                                game.handleWin(2);
                            }
                        }
                    }

                    if (!luigi.getIsDead())
                    {
                        bool wasAlive = true; 
                        enemy->interactWithPlayer(luigi);

                        if (luigi.getIsDead() && wasAlive)
                        {
                            if (luigi.getLives() > 0)
                            {
                                luigiRespawnTimer = respawnDelay;
                            }
                            else
                            {
                                game.handleWin(1);
                            }
                        }
                    }
                }
            }

            /**
             * Mise a jour des joueurs et verification des conditions de victoire
             */
            if (!game.isGameOver())
            {
                mario.update(background.getGroundTiles(), background.getPipes(), background.getQuestionBlocks());

                // Mode IA ou mode normal pour Luigi
                if (isAIMode)
                {
                    // Gestion specifique en mode IA
                    if (!luigi.getIsDead()) 
                    { 
                       
                        aiController->update(
                            background.getGroundTiles(), 
                            background.getPipes(), 
                            background.getFlag()
                        );
                        
                      
                        auto [canMoveRight, canMoveLeft] = luigi.getMouvement().blockMovement(
                            background.getGroundTiles(), 
                            background.getPipes(), 
                            background.getQuestionBlocks()
                        );

                        
                        if (aiController->shouldMoveRight() && canMoveRight) {
                            luigi.moveRight();
                        }
                        if (aiController->shouldMoveLeft() && canMoveLeft) {
                            luigi.moveLeft();
                        }
                        
                        if (aiController->shouldJump() && !luigi.isInAir()) {
                            luigi.jump();
                        }
                        
                        if (aiController->shouldShootFireball() && luigi.hasFirePowerActive()) {
                            luigi.shootFireball();
                        }

                        luigi.applyGravity(background.getGroundTiles(), background.getPipes(), background.getQuestionBlocks());
                        luigi.animate();
                    }
                } 
                else 
                {
                    // Mode normal pour Luigi
                    luigi.update(background.getGroundTiles(), background.getPipes(), background.getQuestionBlocks());
                }

                // Verification des conditions de victoire
                if (mario.checkWin(background.getFlag()) && !game.isGameOver())
                {
                    game.handleWin(1);
                    
                }

                if (luigi.checkWin(background.getFlag()) && !game.isGameOver())
                {
                    game.handleWin(2);
                    if (isAIMode)
                    {
                        std::cout << "Luigi has reached the flag! AI wins!" << std::endl;
                    }
                    else
                    {
                        std::cout << "Luigi has reached the flag! Player 2 wins!" << std::endl;
                    }
                }
            }

            /**
             * Gestion des power-ups: fleurs de feu
             */
            for (FireFlower *flower : flowers)
            {
                // Verification et traitement des collisions avec les fleurs
                if (!flower->isCollected())
                {
                    if (mario.getbounds().intersects(flower->getBounds()))
                    {
                        flower->collect();
                        mario.collectFireFlower();
                    }
                    else if (luigi.getbounds().intersects(flower->getBounds()))
                    {
                        flower->collect();
                        luigi.collectFireFlower();
                    }
                }
            }

            /**
             * Gestion des boules de feu
             */
            // Declenchement des tirs
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
                mario.shootFireball();
            }

            if (!isAIMode && sf::Keyboard::isKeyPressed(sf::Keyboard::F))
            {
                luigi.shootFireball();
            }

            // Mise a jour des boules de feu
            mario.updateFireballs(background.getGroundTiles());
            luigi.updateFireballs(background.getGroundTiles());

            // Gestion des collisions boules de feu - ennemis
            auto checkFireballCollisions = [&](Player &player, const std::string &playerName)
            {
                // Logique de detection et traitement des collisions
                auto &fireballs = player.getFireballs();
                for (auto &fireball : fireballs)
                {
                    if (fireball.isActive())
                    {
                        for (auto &enemy : enemies)
                        {
                            if (enemy->isAlive() && fireball.getBounds().intersects(enemy->getBounds()))
                            {
                                enemy->onFireballHit();
                                fireball.destroy();
                                player.addScore(30); 
                                break;
                            }
                        }
                    }
                }
            };

            checkFireballCollisions(mario, "Mario");
            checkFireballCollisions(luigi, "Luigi");

            /**
             * Gestion des power-ups: etoiles
             */
            for (Etoile *star : etoiles)
            {
                // Verification et traitement des collisions avec les etoiles
                if (!star->isCollected())
                {
                    if (mario.getbounds().intersects(star->getBounds()))
                    {
                        star->collect();
                        mario.collectEtoile();
                    }
                    else if (luigi.getbounds().intersects(star->getBounds()))
                    {
                        star->collect();
                        luigi.collectEtoile();
                    }
                }
            }

            /**
             * Rendu de tous les elements du jeu
             */
            gameWindow.clear();
            background.draw(gameWindow);
            mario.draw(gameWindow);

            // Rendu conditionnel de Luigi selon le mode de jeu
            if (!isAIMode || !luigi.getIsDead())
            { 
                luigi.draw(gameWindow);
                luigi.drawFireballs(gameWindow);
            }

            mario.drawFireballs(gameWindow);
            
            // Rendu des power-ups et collectibles
            for (FireFlower *flower : flowers)
            {
                flower->draw(gameWindow);
            }
            
            for (Etoile *star : etoiles)
            {
                star->draw(gameWindow);
            }

            for (Coin *coin : coins)
            {
                coin->draw(gameWindow);
            }

            /**
             * Mise a jour et rendu des ennemis
             */
            std::vector<Enemy *> enemyPtrs;
            for (auto &enemy : enemies)
            {
                enemyPtrs.push_back(enemy.get());
            }

            // Configuration et verification des interactions ennemis-environnement
            for (auto &enemy : enemies)
            {
                enemy->setCurrentLevel(background.getGroundTiles(),
                                       background.getPipes(),
                                       enemyPtrs); 
                enemy->checkForGaps(background.getGaps());
                enemy->update();
            }

            // Gestion des interactions joueurs-ennemis
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
                        mario.addScore(20); 
                    }
                    else if (luigiRect.intersects(enemyRect) &&
                             luigiRect.top + luigiRect.height <= enemyRect.top + 10)
                    {
                        enemy->onJumpedOn();
                        luigi.bounce();
                        luigi.addScore(20); 
                    }

                    if (enemy->isAlive())
                    {
                        enemy->render(gameWindow);
                    }
                }
            }

            /**
             * Affichage des scores et des vies
             */
            sf::View view = gameWindow.getView();

            static Score marioScore("images/mario_score.png", font, false); 
            static Score luigiScore("images/luigi_score.png", font, false); 
            static Score marioLives("images/mario_score.png", font, true); 
            static Score luigiLives("images/luigi_score.png", font, true); 
            
            // Positionnement des elements d'interface
            float rightSide = view.getCenter().x + view.getSize().x / 2 - 150;
            float leftSide = view.getCenter().x - view.getSize().x / 2 + 20;
            float topPos = view.getCenter().y - view.getSize().y / 2 + 10;

            marioScore.setPosition(rightSide, topPos);
            luigiScore.setPosition(rightSide, topPos + 40);
            marioLives.setPosition(leftSide, topPos);
            luigiLives.setPosition(leftSide, topPos + 40);

            // Mise a jour et affichage des scores et vies
            marioScore.update(mario.getScore());
            luigiScore.update(luigi.getScore());
            marioLives.update(mario.getLives());
            luigiLives.update(luigi.getLives());

            marioScore.draw(gameWindow);
            luigiScore.draw(gameWindow);
            marioLives.draw(gameWindow);
            luigiLives.draw(gameWindow);

            /**
             * Affichage du resultat du jeu si termine
             */
            game.drawResult(gameWindow);
            
            // Mise a jour de la fenetre
            gameWindow.display();
        }

        /**
         * Nettoyage des ressources a la fin de la partie
         */
        if (aiController)
        {
            delete aiController;
            aiController = nullptr;
        }
    }

    return 0;
}