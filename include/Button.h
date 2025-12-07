#pragma once
#include <SFML/Graphics.hpp>

class Button {
public:
    sf::Sprite sprite;

    Button() = default;
    Button(const sf::Texture& texture) { sprite.setTexture(texture); }

    // Devuelve true si el punto (mouse) está sobre el botón
    bool contains(const sf::Vector2f& point) const {
        return sprite.getGlobalBounds().contains(point);
    }
};