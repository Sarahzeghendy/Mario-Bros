#include <SFML/Graphics.hpp>
#include "Headers/player.hpp"
#include "Headers/coin.hpp"
#include "Headers/background.hpp"
#include "Headers/camera.hpp"
#include "Headers/enemy.hpp"
#include "Headers/menu.hpp"
#include "Headers/friendlymushroom.hpp"
#include "Headers/game.hpp"
#include "Headers/fireflower.hpp"
#include "Headers/fireball.hpp"
#include "Headers/AIPlayer.hpp"
#include "Headers/score.hpp"

int main()
{
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

    // Only start the game if menu option was selected
    if (!startGame)
    {
        return 0;
    }

    // Set window title based on game mode
    std::string windowTitle = isAIMode ? "Mario Bros - Joueur contre IA" : "Mario Bros";
    sf::RenderWindow window(sf::VideoMode(1280, 720), windowTitle);
    Background background;
    Game game;
    FireFlower flower(1000, 500);

    // Initialize players
    Player mario("images/sprite.jpg", "Mario", 100, 483, 0.07f, 
                 sf::Keyboard::Right, sf::Keyboard::Left, sf::Keyboard::Up);
    Player luigi("images/sprite.jpg", "Luigi", 200, 480, 0.07f, 
                 sf::Keyboard::M, sf::Keyboard::A, sf::Keyboard::J);

    mario.getSprite().setTextureRect(sf::IntRect(8, 139, 28, 47)); // Starting frame
    luigi.getSprite().setTextureRect(sf::IntRect(8, 191, 28, 47)); // Starting frame

    // Enemies
    std::vector<std::unique_ptr<Enemy>> enemies;
    enemies.push_back(std::make_unique<Goomba>(300, 545));
    enemies.push_back(std::make_unique<KoopaTroopa>(750, 530));
    enemies.push_back(std::make_unique<FriendlyMushroom>(400, 540));

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

    // Game loop
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

        // Process game logic based on mode
        if (isAIMode) {
            // AI mode - update AI controller for Luigi
            aiController->update(background.getGroundTiles(), background.getPipes(), background.getFlag());
            
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
        sf::Vector2f viewCenter = window.getView().getCenter();
        float viewWidth = window.getView().getSize().x;
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
            window.setView(camera.getView());
        }

        // Process enemy interactions
        for (auto &enemy : enemies) {
            if (enemy->isAlive()) {
                enemy->interactWithPlayer(mario);
                
                if (isAIMode) {
                    // Check if Luigi is already small
                    bool wasSmall = luigi.isSmall();
                    
                    // Handle interaction
                    enemy->interactWithPlayer(luigi);
                    
                    // Check if Luigi became small from this hit
                    if (!wasSmall && luigi.isSmall()) {
                        std::cout << "AI Luigi got hit and shrunk!" << std::endl;
                    }
                    
                    // If Luigi was already small and got hit again, handle death
                    if (wasSmall && luigi.isHit() && !luigi.isInvincible()) {
                        std::cout << "AI Luigi died after second hit!" << std::endl;
                        game.handleAIDeath();
                        
                        // Reset Luigi but set as dead
                        luigi.setPosition(-1000, -1000); // Move off-screen
                        luigi.setIsDead(true); // Use setter instead of setDead
                    }
                } else {
                    // Normal interaction for human-controlled Luigi
                    enemy->interactWithPlayer(luigi);
                }
            }
        }

        if (!game.isGameOver()) {
            // Update Mario
            mario.update(background.getGroundTiles(), background.getPipes());
            
            if (isAIMode) {
                // Skip AI update if Luigi is dead
                if (!luigi.getIsDead()) { // Use getter instead of isDead()
                    // For AI mode, Luigi is updated by the AI controller
                    auto [canMoveRight, canMoveLeft] = luigi.getMouvement().blockMovement(
                        background.getGroundTiles(), background.getPipes());

                    // Apply movement based on flags set by AI
                    if (luigi.isMovingRight() && canMoveRight) {
                        luigi.moveRight();
                    }
                    if (luigi.isMovingLeft() && canMoveLeft) {
                        luigi.moveLeft();
                    }

                    // Apply physics
                    luigi.applyGravity(background.getGroundTiles(), background.getPipes());

                    // Animate Luigi
                    luigi.animate();
                }
            } else {
                // Normal mode - update Luigi normally
                luigi.update(background.getGroundTiles(), background.getPipes());
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
                            std::cout << playerName << " a tué un ennemi avec une boule de feu!" << std::endl;
                            break;
                        }
                    }
                }
            }
        };
        
        checkFireballCollisions(mario, "Mario");
        checkFireballCollisions(luigi, "Luigi");

        // Rendering
        window.clear();
        background.draw(window);
        mario.draw(window);
        
        // Only draw Luigi if he's not dead
        if (!isAIMode || !luigi.getIsDead()) { // Use getter instead of isDead()
            luigi.draw(window);
            luigi.drawFireballs(window);
        }
        
        mario.drawFireballs(window);

        flower.draw(window);
        for (Coin *coin : coins) {
            coin->draw(window);
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
                    std::cout << "Mario a sauté sur l'ennemi!" << std::endl;
                }
                else if (luigiRect.intersects(enemyRect) && 
                         luigiRect.top + luigiRect.height <= enemyRect.top + 10) {
                    enemy->onJumpedOn();
                    luigi.bounce();
                    std::cout << "Luigi a sauté sur l'ennemi!" << std::endl;
                }
                else if (enemy->isAlive()) {
                    enemy->interactWithPlayer(mario);
                    
                    if (isAIMode) {
                        enemy->interactWithPlayerSafely(luigi);
                    } else {
                        enemy->interactWithPlayer(luigi);
                    }
                }
                
                if (enemy->isAlive()) {
                    enemy->render(window);
                }
            }
        }

        // Affichage du score
        sf::View view = window.getView();
        float baseX = view.getCenter().x + view.getSize().x/2 - 100;
        float baseY = view.getCenter().y - view.getSize().y/2 + 10;
        
        marioScore.setPosition(baseX, baseY);
        luigiScore.setPosition(baseX, baseY + 40);
        
        marioScore.update(mario.getScore());
        luigiScore.update(luigi.getScore());
        
        marioScore.draw(window);
        luigiScore.draw(window);

        game.drawResult(window);
        window.display();
    }

    if (aiController) {
        delete aiController;
    }

    return 0;
}
