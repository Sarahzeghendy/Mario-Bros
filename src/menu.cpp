#include "Headers/menu.hpp"
#include <iostream>

/**
 * @brief Constructeur de la classe Menu.
 * @param width Largeur de la fenêtre.
 * @param height Hauteur de la fenêtre.
 * @details Charge la police de caractères et les options du menu.
 * @details Initialise le cadre de sélection.
 * @details Charge les images du GIF pour l'animation de fond.
 * @details Positionne l'image de fond et adapte sa taille à la fenêtre.
 */
Menu::Menu(float width, float height) {
    // Charger la police
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        std::cerr << "Erreur : Impossible de charger la police !" << std::endl;
    }

    // Charger les images du GIF (animation de fond)
    int numFrames = 10;  // Modifier en fonction du nombre réel d'images disponibles
    for (int i = 1; i <= numFrames; i++) {
        sf::Texture texture;
        std::string filename = "images/background_frames/frames" + std::to_string(i) + ".png";
        
        if (!texture.loadFromFile(filename)) {
            std::cerr << "Erreur : Impossible de charger " << filename << std::endl;
            continue; // Passer cette image si elle ne peut pas être chargée
        }
        
        frames.push_back(texture);
    }

    if (frames.empty()) {
        std::cerr << "Erreur : Aucune image chargée pour l'animation de fond !" << std::endl;
    } else {
        backgroundSprite.setTexture(frames[0]); // Définir la première image par défaut
    }

    if (!frames.empty()) {
        sf::Vector2u textureSize = frames[0].getSize();
        float scaleX = static_cast<float>(width) / textureSize.x;
        float scaleY = static_cast<float>(height) / textureSize.y;
    
        // ✅ On utilise std::max pour s'assurer que l'image remplit toute la fenêtre
        float finalScale = std::max(scaleX, scaleY);
    
        backgroundSprite.setScale(finalScale, finalScale);
    
        // ✅ Centrage horizontal pour éviter les coupures sur les côtés
        float newPosX = (width - (textureSize.x * finalScale)) / 2.0f;
        float newPosY = (height - (textureSize.y * finalScale)) / 2.0f;
        backgroundSprite.setPosition(newPosX, newPosY);
    }
    
    
    

    // Initialisation des options du menu
    std::vector<std::string> options = {"Jouer", "Regles du jeu", "Quitter"};
    selectedIndex = 0;

    for (size_t i = 0; i < options.size(); i++) {
        sf::Text text;
        text.setFont(font);
        text.setString(options[i]);
        text.setCharacterSize(50);
        text.setFillColor(sf::Color::Black);
        text.setPosition(width / 2 - 100, height / 2 + i * 70); // Ajustement pour être sous le logo

        menuOptions.push_back(text);
    }

    // Initialisation du cadre de sélection
    selector.setSize(sf::Vector2f(250, 60));
    selector.setFillColor(sf::Color(255, 255, 255, 50)); 
    selector.setPosition(menuOptions[selectedIndex].getPosition().x - 20,
                         menuOptions[selectedIndex].getPosition().y);
}

/**
 * @brief Dessine le menu avec une animation de fond.
 * @param window Fenêtre de rendu.
 */
void Menu::draw(sf::RenderWindow &window) {
    // Gérer l'animation du fond (changement d'image toutes les 100ms)
    if (!frames.empty() && frameClock.getElapsedTime().asSeconds() > 0.1f) {
        currentFrame = (currentFrame + 1) % frames.size(); // Passer à l'image suivante
        backgroundSprite.setTexture(frames[currentFrame]);
        frameClock.restart();
    }

    window.draw(backgroundSprite);  // Affichage du fond animé
    window.draw(selector);          // Affichage du cadre de sélection

    for (const auto &option : menuOptions) {
        window.draw(option);
    }
}

/**
 * @brief Déplace le cadre de sélection vers le haut.
 */
void Menu::moveUp() {
    if (selectedIndex > 0) {
        selectedIndex--;
        selector.setPosition(menuOptions[selectedIndex].getPosition().x - 20,
                             menuOptions[selectedIndex].getPosition().y);
    }
}

/**
 * @brief Déplace le cadre de sélection vers le bas.
 */
void Menu::moveDown() {
    if (selectedIndex < static_cast<int>(menuOptions.size()) - 1) {
        selectedIndex++;
        selector.setPosition(menuOptions[selectedIndex].getPosition().x - 20,
                             menuOptions[selectedIndex].getPosition().y);
    }
}

/**
 * @brief Récupère l’indice de l’option sélectionnée.
 * @return L’indice de l’option sélectionnée.
 */
int Menu::getSelectedIndex() const {
    return selectedIndex;
}

