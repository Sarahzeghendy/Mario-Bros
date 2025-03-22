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

    while (restartGame) {
        restartGame = false; // Reset flag
        
        //Musique de fond
        sf::Music music;
        if (!music.openFromFile("musiques/mario.wav")) {
            std::cerr << "Erreur : impossible de charger la musique !\n";
            return -1;
        }

        music.setLoop(true);  // La musique tourne en boucle
        music.play();         // Lancer la musique


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
        FireFlower flower(1000, 500);
        Etoile etoile(450,540); //position

        // Initialize players
        Player mario("images/sprite.jpg", "Mario", 100, 483, 0.07f, 
                     sf::Keyboard::Right, sf::Keyboard::Left, sf::Keyboard::Up);
        Player luigi("images/sprite.jpg", "Luigi", 200, 480, 0.07f, 
                     sf::Keyboard::M, sf::Keyboard::A, sf::Keyboard::J);

        mario.getSprite().setTextureRect(sf::IntRect(8, 139, 28, 47)); // Starting frame
        luigi.getSprite().setTextureRect(sf::IntRect(8, 191, 28, 47)); // Starting frame

        // Enemies
        std::vector<std::unique_ptr<Enemy>> enemies;
        enemies.push_back(std::make_unique<Goomba>(700, 545));
        enemies.push_back(std::make_unique<KoopaTroopa>(750, 530));
        enemies.push_back(std::make_unique<FriendlyMushroom>(400, 540));  // Removed limits

        // Create AI controller for Luigi if in AI mode
        AIPlayer* aiController = nullptr;
        if (isAIMode) {
            aiController = new AIPlayer(&luigi, enemies);
        }

        // Coins
        Coin coin1(300, 500);
        Coin coin2(400, 500);
        Coin coin3(500, 500);
        Coin coin4(600, 500);
        Coin coin5(700, 500);
        Coin* coins[] = {&coin1, &coin2, &coin3, &coin4, &coin5};

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
                
                if (marioRespawnTimer <= 0) {
                    mario.respawn();
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

            // Fire flower collection
            if (mario.getbounds().intersects(flower.getBounds()) && !flower.isCollected()) {
                flower.collect();
                mario.collectFireFlower();
                std::cout << "Mario a collecté une fleur de feu !" << std::endl;
            }

            if (luigi.getbounds().intersects(flower.getBounds()) && !flower.isCollected()) {
                flower.collect();
                luigi.collectFireFlower();
                std::cout << "Luigi a collecté une fleur de feu !" << std::endl;
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

            
            // Etoile
            if (mario.getbounds().intersects(etoile.getBounds()) && !etoile.isCollected()) 
            {
                etoile.collect();
                mario.collectEtoile();
                std::cout << "Mario a collecté une étoile!" << std::endl;
            }

            if (luigi.getbounds().intersects(etoile.getBounds()) && !etoile.isCollected()) {
                etoile.collect();
                luigi.collectEtoile();
                std::cout << "Luigi a collecté une étoile!" << std::endl;
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

            flower.draw(gameWindow);
            etoile.draw(gameWindow);
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
                        mario.addScore(100);  // Award 100 points for stomping an enemy
                        std::cout << "Mario a sauté sur l'ennemi! +100 points" << std::endl;
                    }
                    else if (luigiRect.intersects(enemyRect) && 
                             luigiRect.top + luigiRect.height <= enemyRect.top + 10) {
                        enemy->onJumpedOn();
                        luigi.bounce();
                        luigi.addScore(100);  // Award 100 points for stomping an enemy
                        std::cout << "Luigi a sauté sur l'ennemi! +100 points" << std::endl;
                    }
                    
                    if (enemy->isAlive()) {
                        enemy->render(gameWindow);
                    }
                }
            }

            // Affichage du score
            sf::View view = gameWindow.getView();
            float baseX = view.getCenter().x + view.getSize().x/2 - 100;
            float baseY = view.getCenter().y - view.getSize().y/2 + 10;
            
            marioScore.setPosition(baseX, baseY);
            luigiScore.setPosition(baseX, baseY + 40);
            
            marioScore.update(mario.getScore());
            luigiScore.update(luigi.getScore());
            
            marioScore.draw(gameWindow);
            luigiScore.draw(gameWindow);

            // Display lives along with score
            baseX = view.getCenter().x - view.getSize().x/2 + 20;
            baseY = view.getCenter().y - view.getSize().y/2 + 10;
            
            // Display Mario's lives
            sf::Text marioLivesText;
            marioLivesText.setFont(font);
            marioLivesText.setCharacterSize(24);
            marioLivesText.setFillColor(sf::Color::White);
            marioLivesText.setString("Lives: " + std::to_string(mario.getLives()));
            marioLivesText.setPosition(baseX, baseY);
            gameWindow.draw(marioLivesText);
            
            // Display Luigi's lives
            sf::Text luigiLivesText;
            luigiLivesText.setFont(font);
            luigiLivesText.setCharacterSize(24);
            luigiLivesText.setFillColor(sf::Color::White);
            luigiLivesText.setString("Lives: " + std::to_string(luigi.getLives()));
            luigiLivesText.setPosition(baseX, baseY + 30);
            gameWindow.draw(luigiLivesText);
            
            // Position score displays after the lives
            marioScore.setPosition(baseX + 120, baseY);
            luigiScore.setPosition(baseX + 120, baseY + 30);

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
