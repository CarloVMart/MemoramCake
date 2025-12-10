#pragma once
#include <SFML/Graphics.hpp>

class Button {
public:
    Button();
    Button(const sf::Texture& tex);

    void setTexture(const sf::Texture& tex);
    void setPosition(float x, float y);
    bool isClicked(const sf::RenderWindow& window) const;
    void draw(sf::RenderWindow& window) const;
    sf::FloatRect getBounds() const;

private:
    sf::Sprite sprite;
};