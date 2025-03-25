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



int main()
{
    bool restartGame = true;

    while (restartGame) 
    {
        restartGame = false;
        
        //Musique de fond
        sf::Music music;
        if (!music.openFromFile("musiques/mario.wav")) 
        {
            std::cerr << "Erreur : impossible de charger la musique !\n";
            return -1;
        }

        music.setLoop(true); //musique en boucle 
        music.play();


        // Menu window
        sf::RenderWindow menuWindow(sf::VideoMode(800, 600), "Mario Bros - Menu");
        Menu menu(800, 600);
        bool startGame = false;
        bool isAIMode = false;

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
                        if (choice == 0) // Jouer
                        {
                            startGame = true;
                            isAIMode = false;
                            menuWindow.close();
                        }
                        else if (choice == 1) // Joueur contre IA
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

            menuWindow.clear();
            menu.draw(menuWindow);
            menuWindow.display();
        }

        // If the menu was closed without selecting an option, exit the game
        if (!startGame) {
            return 0;
        }

        // Main game window and setup
        sf::RenderWindow gameWindow(sf::VideoMode(1280, 720), 
                                   isAIMode ? "Mario Bros - Joueur contre IA" : "Mario Bros");

        Background background;
        Game game;
        
        // Add more fire flowers at different positions
        FireFlower flower(900, 370);
        FireFlower flower1(1940, 370);
        FireFlower flower2(6550, 370);  // New flower
        //FireFlower flower3(4100, 300);  // New flower
        //FireFlower flower4(1200, 410);  // New flower
        FireFlower* flowers[] = {&flower, &flower1, &flower2} ;//&flower2, &flower3, &flower4};
        
        // Add more stars at different positions
        Etoile etoile(520, 410);
        Etoile etoile2(3185, 410);
        Etoile etoile3(6235, 410);  // New star
        Etoile etoile4(6870, 410);  // New star
        Etoile etoile5(1190, 410);  // New star
        Etoile etoile6(8485, 410);  // New star
        Etoile* etoiles[] = {&etoile, &etoile2, &etoile5, &etoile3, &etoile4, &etoile6};
        // Initialize players
        Player mario("images/sprite.jpg", "Mario", 100, 483, 0.7f, 
                     sf::Keyboard::Right, sf::Keyboard::Left, sf::Keyboard::Up);
        Player luigi("images/sprite.jpg", "Luigi", 150, 480, 0.7f, 
                     sf::Keyboard::M, sf::Keyboard::A, sf::Keyboard::J);

        mario.getSprite().setTextureRect(sf::IntRect(8, 139, 28, 47)); // Starting frame
        luigi.getSprite().setTextureRect(sf::IntRect(8, 191, 28, 47)); // Starting frame

        // Enemies
        std::vector<std::unique_ptr<Enemy>> enemies;
        enemies.push_back(std::make_unique<Goomba>(700, 545));
        enemies.push_back(std::make_unique<KoopaTroopa>(750, 530));
        enemies.push_back(std::make_unique<Goomba>(1000, 545));
        enemies.push_back(std::make_unique<KoopaTroopa>(1300, 530));
        enemies.push_back(std::make_unique<FriendlyMushroom>(1500, 540));  // Removed limits
        enemies.push_back(std::make_unique<Goomba>(3250, 545));
        enemies.push_back(std::make_unique<KoopaTroopa>(3350, 530));
        enemies.push_back(std::make_unique<FriendlyMushroom>(3300, 540));  // Removed limits
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
        // Create AI controller for Luigi if in AI mode
        AIPlayer* aiController = nullptr;
        if (isAIMode) {
            aiController = new AIPlayer(&luigi, enemies);
        }

        // Coins
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
        Coin* coins[] = {&coin1, &coin2, &coin3, &coin4, &coin5, &coin6, &coin7, &coin8, &coin9, &coin10, &coin11, &coin12, &coin13, &coin14, &coin15, &coin16, &coin17, &coin18, &coin19, &coin20, &coin21, &coin22};

        // Camera setup
        Camera camera(800, 600);

        sf::Font font;
        if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) // Chargement du font pour l'affichage du score
        {
            std::cout << "Error loading font" << std::endl;
            return -1;
        }

        // Replace scoreText with Score objects
        Score marioScore("images/mario_score.png", font);
        Score luigiScore("images/luigi_score.png", font);

        // Add a respawn timer
        const int respawnDelay = 120; // 2 seconds at 60 FPS
        int marioRespawnTimer = 0;
        int luigiRespawnTimer = 0;
        
        // Add status text for respawning players
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

        // Game loop
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

            
            //Gestion de la collision entre Mario et Luigi

            //vérification collision entre Mario et Luigi
            if (mario.getbounds().intersects(luigi.getbounds())) 
            {
                //si mario se retrouve au dessus de luigi -> mario saute sur luigi
                if (mario.getPosition().y + mario.getbounds().height <= luigi.getPosition().y + 5) 
                {
                    // Mario saute sur Luigi
                    mario.getMouvement().jump();

                } 
                // si Luigi se trouve au-dessus de Mario, Luigi saute sur Mario
                else if (luigi.getPosition().y + luigi.getbounds().height <= mario.getPosition().y + 5) 
                {
                    // Luigi saute sur Mario
                    luigi.getMouvement().jump();

                }
                else 
                {
                    //gestion des déplacements horizontaux

                    //vérification mario bouge
                    bool marioBouge = mario.isMoving();

                    //vérification luigi bouge
                    bool luigiBouge = luigi.isMoving();
                    
                     // bloquer la traversée horizontale (qu'ils se chevauchent)
                    if (luigiBouge) 
                    {
                        // si Luigi se déplace vers la droite et entre en collision avec Mario
                        if (luigi.getPosition().x < mario.getPosition().x) 
                        {
                            std::cout << "Luigi va vers la droite, collision avec Mario\n";
                            //Luigi placé juste à gauche de Mario
                            luigi.setPosition(mario.getPosition().x - luigi.getbounds().width, luigi.getPosition().y);
                        } 
                        //si Luigi se déplace vers la gauche et entre en collision avec Mario
                        else if (luigi.getPosition().x > mario.getPosition().x) 
                        {
                            std::cout << "Luigi va vers la gauche, collision avec Mario\n";
                            //Luigi placé juste à droite de Mario
                            luigi.setPosition(mario.getPosition().x + mario.getbounds().width, luigi.getPosition().y);
                        }
                    }

                    if (marioBouge) 
                    {
                        //si Mario se déplace vers la droite et entre en collision avec Luigi
                        if (mario.getPosition().x < luigi.getPosition().x) 
                        {
                            std::cout << "Mario va vers la droite, collision avec Luigi\n";
                            //Mario placé juste à gauche de Luigi
                            mario.setPosition(luigi.getPosition().x - mario.getbounds().width, mario.getPosition().y);
                        } 
                        //si Mario se déplace vers la gauche et entre en collision avec Luigi
                        else if (mario.getPosition().x > luigi.getPosition().x) 
                        {
                            std::cout << "Mario va vers la gauche, collision avec Luigi\n";
                            //Mario placé juste à droite de Luigi
                            mario.setPosition(luigi.getPosition().x + luigi.getbounds().width, mario.getPosition().y);
                        }
                    }


                }
            }




            // Check if a player falls below the lower limit of the screen
            float lowerLimit = gameWindow.getSize().y + 50; // Allow some margin below the screen
            if (mario.getPosition().y > lowerLimit && !mario.getIsDead()) {
                mario.loseLife();
                if (mario.getLives() <= 0) {
                    game.handleWin(2); // Luigi wins if Mario has no lives
                } else {
                    marioRespawnTimer = respawnDelay;
                }
            }
            
            if (luigi.getPosition().y > lowerLimit && !luigi.getIsDead()) {
                luigi.loseLife();
                if (luigi.getLives() <= 0) {
                    game.handleWin(1); // Mario wins if Luigi has no lives
                } else {
                    luigiRespawnTimer = respawnDelay;
                }
            }

            // Handle respawning
            if (mario.shouldRespawn() && marioRespawnTimer > 0) {
                marioRespawnTimer--;
                
                // Show respawn countdown
                marioRespawnText.setString("Mario respawning in " + std::to_string((marioRespawnTimer / 60) + 1) + "...");
                marioRespawnText.setPosition(mario.getPosition().x - 50, mario.getPosition().y - 40);
                
                // Add debugging
                std::cout << "Mario respawn countdown: " << marioRespawnTimer << " (isDead=" 
                          << mario.getIsDead() << ", lives=" << mario.getLives() 
                          << ", fire power=" << mario.hasFirePowerActive() << ")" << std::endl;
                
                if (marioRespawnTimer <= 0) {
                    std::cout << "Calling mario.respawn()..." << std::endl;
                    mario.respawn();
                    // Verify respawn worked
                    std::cout << "After respawn: isDead=" << mario.getIsDead() << std::endl;
                }
            }

            if (luigi.shouldRespawn() && luigiRespawnTimer > 0) {
                luigiRespawnTimer--;
                
                // Show respawn countdown
                luigiRespawnText.setString("Luigi respawning in " + std::to_string((luigiRespawnTimer / 60) + 1) + "...");
                luigiRespawnText.setPosition(luigi.getPosition().x - 50, luigi.getPosition().y - 40);
                
                if (luigiRespawnTimer <= 0) {
                    luigi.respawn();
                }
            }

            // If the game is over, display the winner and return to the menu
            if (game.isGameOver()) {
                // Create a more visual button rather than just text
                sf::Text returnToMenuText;
                returnToMenuText.setFont(font);
                returnToMenuText.setString("Return to Menu");
                returnToMenuText.setCharacterSize(30);
                returnToMenuText.setFillColor(sf::Color::White);

                // Create button background
                sf::RectangleShape menuButton;
                menuButton.setSize(sf::Vector2f(300, 60));
                menuButton.setFillColor(sf::Color(50, 50, 200, 230)); // Blue semi-transparent 
                menuButton.setOutlineThickness(3);
                menuButton.setOutlineColor(sf::Color::White);

                // Position the elements
                sf::Vector2f viewCenter = gameWindow.getView().getCenter();
                
                // Center the button
                menuButton.setOrigin(menuButton.getSize().x / 2, menuButton.getSize().y / 2);
                menuButton.setPosition(viewCenter.x, viewCenter.y + 80);
                
                // Center the text on the button
                sf::FloatRect textBounds = returnToMenuText.getLocalBounds();
                returnToMenuText.setOrigin(textBounds.width / 2, textBounds.height / 2);
                returnToMenuText.setPosition(viewCenter.x, viewCenter.y + 80);

                // Game over loop
                bool buttonHovered = false;
                while (gameWindow.isOpen()) {
                    sf::Event event;
                    while (gameWindow.pollEvent(event)) {
                        if (event.type == sf::Event::Closed) {
                            gameWindow.close();
                            return 0;
                        }
                        
                        // Check for mouse position for hover effect
                        sf::Vector2i mousePos = sf::Mouse::getPosition(gameWindow);
                        sf::Vector2f worldPos = gameWindow.mapPixelToCoords(mousePos);
                        
                        if (menuButton.getGlobalBounds().contains(worldPos)) {
                            if (!buttonHovered) {
                                // Mouse entered the button - highlight it
                                menuButton.setFillColor(sf::Color(100, 100, 255, 230)); // Lighter blue
                                menuButton.setOutlineColor(sf::Color::Yellow);
                                returnToMenuText.setFillColor(sf::Color::Yellow);
                                buttonHovered = true;
                            }
                            
                            // Check for click
                            if (event.type == sf::Event::MouseButtonPressed && 
                                event.mouseButton.button == sf::Mouse::Left) {
                                // Button clicked - return to menu
                                std::cout << "Return to menu button clicked!" << std::endl;
                                gameWindow.close(); // Close the game window
                                restartGame = true; // Set flag to restart from menu
                                break;
                            }
                        } else if (buttonHovered) {
                            // Mouse left the button - reset it
                            menuButton.setFillColor(sf::Color(50, 50, 200, 230)); // Back to original color
                            menuButton.setOutlineColor(sf::Color::White);
                            returnToMenuText.setFillColor(sf::Color::White);
                            buttonHovered = false;
                        }
                    }

                    if (restartGame) {
                        break; // Exit the game over loop if returning to menu
                    }

                    // Draw the game over screen
                    gameWindow.clear();
                    background.draw(gameWindow);
                    game.drawResult(gameWindow);
                    
                    // Draw the button first, then the text on top
                    gameWindow.draw(menuButton);
                    gameWindow.draw(returnToMenuText);
                    
                    // Add instructions
                    sf::Text instructionText;
                    instructionText.setFont(font);
                    instructionText.setString("Click the button to return to menu");
                    instructionText.setCharacterSize(20);
                    instructionText.setFillColor(sf::Color::White);
                    instructionText.setPosition(viewCenter.x - 150, viewCenter.y + 130);
                    gameWindow.draw(instructionText);
                    
                    gameWindow.display();
                }
                
                if (restartGame) {
                    break; // Exit the main game loop if returning to menu
                }
            }

            // Process game logic based on mode
            if (isAIMode) {
                // AI mode - update AI controller for Luigi
                aiController->update(background.getGroundTiles(), background.getPipes(), background.getFlag()); // Fixed argument
                
                // Handle human input for Mario only
                mario.setMovingRight(sf::Keyboard::isKeyPressed(sf::Keyboard::Right));
                mario.setMovingLeft(sf::Keyboard::isKeyPressed(sf::Keyboard::Left));
                
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !mario.isInAir()) {
                    mario.jump();
                }
                
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && mario.hasFirePowerActive()) {
                    mario.shootFireball();
                }
            } else {
                // Normal 2-player mode
                // Both players respond to their respective controls
            }

            // Common code for both modes - check coin collection
            for (Coin* coin : coins) {
                if (!coin->isCollected()) {
                    if (mario.getbounds().intersects(coin->getBounds())) {
                        coin->collect();
                        mario.getCoins();
                        std::cout << "Mario a collecté une pièce !" << std::endl;
                    }
                    if (luigi.getbounds().intersects(coin->getBounds())) {
                        coin->collect();
                        luigi.getCoins();
                        std::cout << "Luigi a collecté une pièce !" << std::endl;
                    }
                }
            }

            // Camera handling
            sf::Vector2f viewCenter = gameWindow.getView().getCenter();
            float viewWidth = gameWindow.getView().getSize().x;
            float screenLeft = viewCenter.x - (viewWidth / 2);
            float screenRight = viewCenter.x + (viewWidth / 2);

            sf::Vector2f marioPos = mario.getPosition();
            sf::Vector2f luigiPos = luigi.getPosition();

            // Keep players on screen
            float margin = 80.0f;
            if (marioPos.x < screenLeft + margin) {
                mario.setPosition(screenLeft + margin, marioPos.y);
            }
            if (marioPos.x > screenRight - margin) {
                mario.setPosition(screenRight - margin, marioPos.y);
            }

            if (luigiPos.x < screenLeft + margin) {
                luigi.setPosition(screenLeft + margin, luigiPos.y);
            }
            if (luigiPos.x > screenRight - margin) {
                luigi.setPosition(screenRight - margin, luigiPos.y);
            }

            // Update camera position
            float camX = (marioPos.x + luigiPos.x) / 2.0f;
            float camY = 350.0f;

            float playerDistance = std::abs(marioPos.x - luigiPos.x);
            if (playerDistance < viewWidth * 0.8f) { 
                camera.setCenter(camX, camY);
                gameWindow.setView(camera.getView());
            }

            // Process enemy interactions
            for (auto &enemy : enemies) {
                if (enemy->isAlive()) {
                    // Check if Mario is already dead
                    if (!mario.getIsDead()) {
                        bool wasAlive = true; // Mario is alive at this point
                        enemy->interactWithPlayer(mario);
                        
                        // If Mario is now dead but was alive before the collision
                        if (mario.getIsDead() && wasAlive) {
                            std::cout << "Mario died from enemy collision!" << std::endl;
                            // Only set the respawn timer if player still has lives
                            if (mario.getLives() > 0) {
                                marioRespawnTimer = respawnDelay;
                                std::cout << "Mario will respawn in " << respawnDelay/60 << " seconds" << std::endl;
                            } else {
                                // Game over - Luigi wins
                                game.handleWin(2);
                            }
                        }
                    }
                    
                    // Same logic for Luigi
                    if (!luigi.getIsDead()) {
                        bool wasAlive = true; // Luigi is alive at this point
                        enemy->interactWithPlayer(luigi);
                        
                        // If Luigi died from this hit
                        if (luigi.getIsDead() && wasAlive) {
                            std::cout << "Luigi died from enemy collision!" << std::endl;
                            // Only set the respawn timer if player still has lives
                            if (luigi.getLives() > 0) {
                                luigiRespawnTimer = respawnDelay;
                                std::cout << "Luigi will respawn in " << respawnDelay/60 << " seconds" << std::endl;
                            } else {
                                // Game over - Mario wins
                                game.handleWin(1);
                            }
                        }
                    }
                }
            }

            // If the game is over, display the winner and return to the menu
            if (game.isGameOver()) {
                sf::Text returnToMenuText;
                returnToMenuText.setFont(font);
                returnToMenuText.setString("Return to the Menu");
                returnToMenuText.setCharacterSize(30);
                returnToMenuText.setFillColor(sf::Color::White);

                sf::Vector2f viewCenter = gameWindow.getView().getCenter();
                sf::FloatRect textBounds = returnToMenuText.getLocalBounds();
                returnToMenuText.setOrigin(textBounds.width / 2, textBounds.height / 2);
                returnToMenuText.setPosition(viewCenter.x, viewCenter.y + 50);

                while (gameWindow.isOpen()) {
                    sf::Event event;
                    while (gameWindow.pollEvent(event)) {
                        if (event.type == sf::Event::Closed) {
                            gameWindow.close();
                            return 0;
                        }
                        if (event.type == sf::Event::MouseButtonPressed) {
                            sf::Vector2i mousePos = sf::Mouse::getPosition(gameWindow);
                            sf::FloatRect buttonBounds = returnToMenuText.getGlobalBounds();
                            if (buttonBounds.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                                // Return to the menu
                                gameWindow.close(); // Close the game window
                                restartGame = true; // Set flag to restart from menu
                                break;
                            }
                        }
                    }

                    if (restartGame) {
                        break; // Exit the game over loop if returning to menu
                    }

                    gameWindow.clear();
                    background.draw(gameWindow);
                    game.drawResult(gameWindow);
                    gameWindow.draw(returnToMenuText);
                    gameWindow.display();
                }
                
                if (restartGame) {
                    break; // Exit the main game loop if returning to menu
                }
            }

            if (!game.isGameOver()) {
                // Update Mario
                mario.update(background.getGroundTiles(), background.getPipes(), background.getQuestionBlocks()); // Added questionBlocks
                
                if (isAIMode) {
                    // Skip AI update if Luigi is dead
                    if (!luigi.getIsDead()) { // Use getter instead of isDead()
                        // For AI mode, Luigi is updated by the AI controller
                        auto [canMoveRight, canMoveLeft] = luigi.getMouvement().blockMovement(
                            background.getGroundTiles(), background.getPipes(), background.getQuestionBlocks()); // Added questionBlocks

                        // Apply movement based on flags set by AI
                        if (luigi.isMovingRight() && canMoveRight) {
                            luigi.moveRight();
                        }
                        if (luigi.isMovingLeft() && canMoveLeft) {
                            luigi.moveLeft();
                        }

                        // Apply physics
                        luigi.applyGravity(background.getGroundTiles(), background.getPipes(), background.getQuestionBlocks()); // Added questionBlocks

                        // Animate Luigi
                        luigi.animate();
                    }
                } else {
                    // Normal mode - update Luigi normally
                    luigi.update(background.getGroundTiles(), background.getPipes(), background.getQuestionBlocks()); // Added questionBlocks
                }

                // Check win conditions
                if (mario.checkWin(background.getFlag()) && !game.isGameOver()) {
                    game.handleWin(1); 
                    std::cout << "Mario has reached the flag! Player 1 wins!" << std::endl;
                }

                if (luigi.checkWin(background.getFlag()) && !game.isGameOver()) {
                    game.handleWin(2); 
                    if (isAIMode) {
                        std::cout << "Luigi has reached the flag! AI wins!" << std::endl;
                    } else {
                        std::cout << "Luigi has reached the flag! Player 2 wins!" << std::endl;
                    }
                }
            }

            // Fire flower collection - check all flowers for both players
            for (FireFlower* flower : flowers) {
                if (!flower->isCollected()) {
                    if (mario.getbounds().intersects(flower->getBounds())) {
                        flower->collect();
                        mario.collectFireFlower();
                        std::cout << "Mario a collecté une fleur de feu !" << std::endl;
                    }
                    else if (luigi.getbounds().intersects(flower->getBounds())) {
                        flower->collect();
                        luigi.collectFireFlower();
                        std::cout << "Luigi a collecté une fleur de feu !" << std::endl;
                    }
                }
            }
            
            // Fireball input handling
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                mario.shootFireball();
            }

            if (!isAIMode && sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
                luigi.shootFireball();
            }

            // Update fireballs
            mario.updateFireballs(background.getGroundTiles());
            luigi.updateFireballs(background.getGroundTiles());

            // Check fireball collisions with enemies
            auto checkFireballCollisions = [&](Player& player, const std::string& playerName) {
                auto& fireballs = player.getFireballs();
                for (auto& fireball : fireballs) {
                    if (fireball.isActive()) {
                        for (auto& enemy : enemies) {
                            if (enemy->isAlive() && fireball.getBounds().intersects(enemy->getBounds())) {
                                enemy->onFireballHit(); 
                                fireball.destroy();
                                player.addScore(50);  // Award 50 points for killing an enemy with fireball
                                std::cout << playerName << " a tué un ennemi avec une boule de feu! +50 points" << std::endl;
                                break;
                            }
                        }
                    }
                }
            };
            
            checkFireballCollisions(mario, "Mario");
            checkFireballCollisions(luigi, "Luigi");

            
            // Star collection - check all stars for both players
            for (Etoile* star : etoiles) {
                if (!star->isCollected()) {
                    if (mario.getbounds().intersects(star->getBounds())) {
                        star->collect();
                        mario.collectEtoile();
                        std::cout << "Mario a collecté une étoile!" << std::endl;
                    }
                    else if (luigi.getbounds().intersects(star->getBounds())) {
                        star->collect();
                        luigi.collectEtoile();
                        std::cout << "Luigi a collecté une étoile!" << std::endl;
                    }
                }
            }




            // Rendering
            gameWindow.clear();
            background.draw(gameWindow);
            mario.draw(gameWindow);
            
            // Only draw Luigi if he's not dead
            if (!isAIMode || !luigi.getIsDead()) { // Use getter instead of isDead()
                luigi.draw(gameWindow);
                luigi.drawFireballs(gameWindow);
            }
            
            mario.drawFireballs(gameWindow);

            // Draw all flowers
            for (FireFlower* flower : flowers) {
                flower->draw(gameWindow);
            }
            
            // Draw all stars
            for (Etoile* star : etoiles) {
                star->draw(gameWindow);
            }
            
            // Draw coins
            for (Coin *coin : coins) {
                coin->draw(gameWindow);
            }

            // Inside the game loop, before updating enemies
            std::vector<Enemy*> enemyPtrs;
            for (auto& enemy : enemies) {
                enemyPtrs.push_back(enemy.get());
            }

            // Update each enemy with current level information
            for (auto& enemy : enemies) {
                enemy->setCurrentLevel(background.getGroundTiles(), 
                                       background.getPipes(),
                                       enemyPtrs); // Call setCurrentLevel
                enemy->checkForGaps(background.getGaps());
                enemy->update(); 
            }

            for (auto &enemy : enemies) {
                if (enemy->isAlive()) {
                    enemy->checkForGaps(background.getGaps());
                    enemy->update();
                    
                    // Check if players are jumping on enemies
                    sf::FloatRect marioRect = mario.getbounds();
                    sf::FloatRect luigiRect = luigi.getbounds();
                    sf::FloatRect enemyRect = enemy->getBounds();
                    
                    if (marioRect.intersects(enemyRect) && 
                        marioRect.top + marioRect.height <= enemyRect.top + 10) {
                        enemy->onJumpedOn();
                        mario.bounce();
                        mario.addScore(30);  // Award 100 points for stomping an enemy
                        std::cout << "Mario a sauté sur l'ennemi! +100 points" << std::endl;
                    }
                    else if (luigiRect.intersects(enemyRect) && 
                             luigiRect.top + luigiRect.height <= enemyRect.top + 10) {
                        enemy->onJumpedOn();
                        luigi.bounce();
                        luigi.addScore(30);  // Award 100 points for stomping an enemy
                        std::cout << "Luigi a sauté sur l'ennemi! +100 points" << std::endl;
                    }
                    
                    if (enemy->isAlive()) {
                        enemy->render(gameWindow);
                    }
                }
            }

            // Score and Lives Display
            sf::View view = gameWindow.getView();

            // Initialize Score displays - regular scores show numbers
            static Score marioScore("images/mario_score.png", font, false); // false = score display
            static Score luigiScore("images/luigi_score.png", font, false); // false = score display

            // Initialize Lives displays - these show hearts
            static Score marioLives("images/mario_score.png", font, true);  // true = lives display 
            static Score luigiLives("images/luigi_score.png", font, true);  // Use luigi_score.png instead of mario_score.png

            // Position the displays on screen
            float rightSide = view.getCenter().x + view.getSize().x/2 - 150;
            float leftSide = view.getCenter().x - view.getSize().x/2 + 20;
            float topPos = view.getCenter().y - view.getSize().y/2 + 10;

            // Set positions
            marioScore.setPosition(rightSide, topPos);
            luigiScore.setPosition(rightSide, topPos + 40);
            marioLives.setPosition(leftSide, topPos);
            luigiLives.setPosition(leftSide, topPos + 40);

            // Update values
            marioScore.update(mario.getScore());
            luigiScore.update(luigi.getScore());
            marioLives.update(mario.getLives());
            luigiLives.update(luigi.getLives());

            // Draw all UI elements
            marioScore.draw(gameWindow);
            luigiScore.draw(gameWindow);
            marioLives.draw(gameWindow);
            luigiLives.draw(gameWindow);

            // Draw respawn text if players are respawning
            if (mario.shouldRespawn() && marioRespawnTimer > 0) {
                gameWindow.draw(marioRespawnText);
            }
            
            if (luigi.shouldRespawn() && luigiRespawnTimer > 0) {
                gameWindow.draw(luigiRespawnText);
            }

            game.drawResult(gameWindow);
            gameWindow.display();
        }

        // Clean up resources
        if (aiController) {
            delete aiController;
            aiController = nullptr;
        }
    }

    return 0;
}