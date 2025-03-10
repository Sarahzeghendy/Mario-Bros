#include <SFML/Graphics.hpp>
#include "Headers/background.hpp"
#include "Headers/player1.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Mario Bros");
    Background background;
    
   
    Player player("images/mario.png", 100, 300, 0.07f, sf::Keyboard::D, sf::Keyboard::Q, sf::Keyboard::Space);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        
        player.update(background.getGroundTiles(), background.getPipes());
        player.applyGravity(background.getGroundTiles(), background.getPipes());

        window.clear();

        background.draw(window);
        player.draw(window);

        window.display();
    }

    return 0;
}
