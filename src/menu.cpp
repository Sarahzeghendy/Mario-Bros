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
Menu::Menu(float width, float height)
{

    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"))
    {
        std::cerr << "Erreur : Impossible de charger la police !" << std::endl;
    }

    int numFrames = 10;
    for (int i = 1; i <= numFrames; i++)
    {
        sf::Texture texture;
        std::string filename = "images/background_frames/frames" + std::to_string(i) + ".png";

        if (!texture.loadFromFile(filename))
        {
            std::cerr << "Erreur : Impossible de charger " << filename << std::endl;
            continue;
        }

        frames.push_back(texture);
    }

    if (frames.empty())
    {
        std::cerr << "Erreur : Aucune image chargée pour l'animation de fond !" << std::endl;
    }
    else
    {
        backgroundSprite.setTexture(frames[0]);
    }

    if (!frames.empty())
    {
        sf::Vector2u textureSize = frames[0].getSize();
        float scaleX = static_cast<float>(width) / textureSize.x;
        float scaleY = static_cast<float>(height) / textureSize.y;

        float finalScale = std::max(scaleX, scaleY);

        backgroundSprite.setScale(finalScale, finalScale);

        float newPosX = (width - (textureSize.x * finalScale)) / 2.0f;
        float newPosY = (height - (textureSize.y * finalScale)) / 2.0f;
        backgroundSprite.setPosition(newPosX, newPosY);
    }

    // les 4 cases presentent dans le menu
    std::vector<std::string> options = {"Jouer", "Jouer contre IA", "Regles du jeu", "Quitter"};
    selectedIndex = 0;

    for (size_t i = 0; i < options.size(); i++)
    {
        sf::Text text;
        text.setFont(font);
        text.setString(options[i]);
        text.setCharacterSize(50);
        text.setFillColor(sf::Color::Black);
        text.setPosition(width / 2 - 100, height / 2 + i * 70);

        menuOptions.push_back(text);
    }

    selector.setSize(sf::Vector2f(250, 60));
    selector.setFillColor(sf::Color(255, 255, 255, 50));
    selector.setPosition(menuOptions[selectedIndex].getPosition().x - 20,
                         menuOptions[selectedIndex].getPosition().y);
}

/**
 * @brief Dessine le menu avec une animation de fond.
 * @param window Fenêtre de rendu.
 */
void Menu::draw(sf::RenderWindow &window)
{

    if (!frames.empty() && frameClock.getElapsedTime().asSeconds() > 0.1f)
    {
        currentFrame = (currentFrame + 1) % frames.size();
        backgroundSprite.setTexture(frames[currentFrame]);
        frameClock.restart();
    }

    window.draw(backgroundSprite);
    window.draw(selector);

    for (const auto &option : menuOptions)
    {
        window.draw(option);
    }
}

/**
 * @brief Deplace le cadre de selection vers le haut
 * @details Si l'option selectionnee nest pas la premiere deplace le cadre de selection vers le haut
 * @details et decremente lindice de loption selectionnee
 */
void Menu::moveUp()
{
    if (selectedIndex > 0)
    {
        selectedIndex--;
        selector.setPosition(menuOptions[selectedIndex].getPosition().x - 20,
                             menuOptions[selectedIndex].getPosition().y);
    }
}

/**
 * @brief Deplace le cadre de selection vers le bas
 * @details Si loption selectionnee nest pas la derniere deplace le cadre de selection vers le bas
 * @details et incremente lindice de loption selectionnee
 */
void Menu::moveDown()
{
    if (selectedIndex < static_cast<int>(menuOptions.size()) - 1)
    {
        selectedIndex++;
        selector.setPosition(menuOptions[selectedIndex].getPosition().x - 20,
                             menuOptions[selectedIndex].getPosition().y);
    }
}

/**
 * @brief Recupere lindice de loption selectionnee
 * @return Lindice de loption selectionnee
 */
int Menu::getSelectedIndex() const
{
    return selectedIndex;
}

/**
 * @brief Affiche les regles du jeu dans une nouvelle fenetre.
 */
void Menu::showRules()
{
    sf::RenderWindow rulesWindow(sf::VideoMode(900, 500), "Regles du jeu");

    sf::Text rulesText;
    rulesText.setFont(font);
    rulesText.setString(
        "Regles du jeu Mario Bros :\n\n"
        "1. Deplacez Mario avec les fleches gauche, droite et la fleche up pour sauter.\n"
        "2. Deplacez luigi avec les touches D, Q et Z pour sauter.\n"
        "3. Evitez les trous et les ennemis pour ne pas perdre de vies.\n"
        "4. Ramassez les pieces pour gagner des points et des vies.\n"
        "5. Atteignez le drapeau pour gagner le niveau.\n"
        "6. Collectez les fleurs de feu pour obtenir des boules de feu.\n"
        "7. Appuyez sur espace (Mario) pour tirer des boules de feu.\n"
        "8. Appuyez sur f (Luigi) pour tirer des boules de feu.\n"
        "9. Collectez les etoiles pour devenir plus rapide.\n"
        "10. Collectez les champignons pour devenir plus grand.\n");
    rulesText.setCharacterSize(20);
    rulesText.setFillColor(sf::Color::Black);
    rulesText.setPosition(20, 20);

    while (rulesWindow.isOpen())
    {
        sf::Event event;
        while (rulesWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                rulesWindow.close();
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            {
                rulesWindow.close();
            }
        }

        rulesWindow.clear(sf::Color(240, 240, 240));
        rulesWindow.draw(rulesText);
        rulesWindow.display();
    }
}
