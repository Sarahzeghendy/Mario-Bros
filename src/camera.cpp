#include "Headers/camera.hpp"


/**
 * @brief Constructeur de la classe Camera
 * @param width Largeur de la camera
 * @param height Hauteur de la camera
 * @details Initialise la taille de la vue et définit les limites du déplacement de la caméra
 */
Camera::Camera(float width, float height) : 
    defaultWidth(width), defaultHeight(height),
    minX(width / 2), maxX(8400),//Limites horizontales en fonction de la taille du niveau
    minY(height / 2), maxY(600) //Limites verticales en fonction de la taille du niveau
{
    view.setSize(width, height);  

    //définition position initiale             
    view.setCenter(width / 2.0f, height / 2.0f);
}


/**
 * @brief Met à jour la taille de la vue en fonction de la taille de la fenêtre
 * @param windowSize Dimensions de la fenêtre
 * @details Ajuste la vue pour maintenir un ratio d'affichage correct
 */
void Camera::update(const sf::Vector2u &windowSize)
{
    //calcul ratio aspect fenêtre -> bonne proportion
    float aspectRatio = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);
    
    //modification taille vue en fonction ratio calculé
    view.setSize(defaultHeight * aspectRatio, defaultHeight);
}


/**
 * @brief Retourne la vue actuelle de la caméra
 * @return La vue de la caméra
 */
const sf::View &Camera::getView() const
{
    return view;
}

/**
 * @brief Contraint la position de la caméra dans les limites définies
 * @param x Coordonnée horizontale à contraindre
 * @param y Coordonnée verticale à contraindre
 * @details Empêche la caméra de sortir des bornes définies
 */
void Camera::clampPosition(float &x, float &y)
{
    // contrainte x pour qu'il soit entre minX et maxX
    x = std::max(minX, std::min(x, maxX));

    // contrainte y pour qu'il soit entre minY et maxY
    y = std::max(minY, std::min(y, maxY));
}

/**
 * @brief Définit le centre de la caméra
 * @param x Nouvelle position horizontale du centre de la caméra
 * @param y Nouvelle position verticale du centre de la caméra
 * @details Applique une contrainte sur la position avant de centrer la vue
 */
void Camera::setCenter(float x, float y)
{
    //modification x et y pour les ramener dans les bornes autorisées
    clampPosition(x, y); 

    //position centre de la vue nouvelle position
    view.setCenter(x, y);
}
