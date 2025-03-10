#include "Headers/camera.hpp"



Camera::Camera(float width, float height) : defaultWidth(width), defaultHeight(height),
      minX(width/2), maxX(2000), // Adjust these values based on your level size
      minY(height/2), maxY(600)  // Adjust these values based on your level size
{
    view.setSize(width, height); //Définit la taille de la vue avec les dimensions données.
    view.setCenter(width / 2.0f, height / 2.0f); //vue au centre
}


//lorsqu'on redimensionne la fenêtre, la caméra ajuste sa largeur pour respecter ratio d'aspect
void Camera::update(const sf::Vector2u& windowSize) 
{
    float aspectRatio = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);
    view.setSize(defaultHeight * aspectRatio, defaultHeight);
}

const sf::View& Camera::getView() const 
{
    return view;
}

void Camera::clampPosition(float& x, float& y) {
    float halfWidth = view.getSize().x / 2.0f;
    float halfHeight = view.getSize().y / 2.0f;
    
    x = std::max(minX, std::min(x, maxX));
    y = std::max(minY, std::min(y, maxY));
}

void Camera::setCenter(float x, float y) {
    clampPosition(x, y);
    view.setCenter(x, y);
}
