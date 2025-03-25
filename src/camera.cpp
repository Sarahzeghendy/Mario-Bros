#include "Headers/camera.hpp"

/**
 * @brief Constructeur de la classe Camera
 * @param width Largeur initiale de la vue
 * @param height Hauteur initiale de la vue
 */
Camera::Camera(float width, float height) : 
    defaultWidth(width), defaultHeight(height),
    minX(width / 2), maxX(8400), // Limites horizontales en fonction de la taille du niveau
    minY(height / 2), maxY(600)  // Limites verticales pour garder la vue centree
{
    view.setSize(width, height);                 
    view.setCenter(width / 2.0f, height / 2.0f); // Position initiale au debut du niveau
}

/**
 * @brief Ajuste la vue lors du redimensionnement de la fenetre
 * @param windowSize Nouvelles dimensions de la fenetre
 * @details
 * - Maintien du ratio d'aspect pour eviter les deformations visuelles
 */
void Camera::update(const sf::Vector2u &windowSize)
{
    float aspectRatio = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);
    view.setSize(defaultHeight * aspectRatio, defaultHeight);
    
}

/**
 * @brief Retourne la vue actuelle de la camera
 * @return Reference constante vers la vue SFML
 */
const sf::View &Camera::getView() const
{
    return view;
}

/**
 * @brief Contraint une position dans les limites definies pour la camera
 * @param x Position horizontale a contraindre (modifiee par reference)
 * @param y Position verticale a contraindre (modifiee par reference)
 */
void Camera::clampPosition(float &x, float &y)
{
    // La camera ne peut pas montrer les zones hors des limites du niveau
    x = std::max(minX, std::min(x, maxX));
    y = std::max(minY, std::min(y, maxY));
}

/**
 * @brief Positionne le centre de la camera
 * @param x Nouvelle position horizontale souhaitee
 * @param y Nouvelle position verticale souhaitee
 */
void Camera::setCenter(float x, float y)
{
    clampPosition(x, y); // Applique les limites avant de centrer
    view.setCenter(x, y);
}
