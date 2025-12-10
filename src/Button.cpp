#include "Button.hpp"

Button::Button() {}
Button::Button(const sf::Texture& tex) { sprite.setTexture(tex); }

void Button::setTexture(const sf::Texture& tex) { sprite.setTexture(tex); }
void Button::setPosition(float x, float y) { sprite.setPosition(x, y); }

bool Button::isClicked(const sf::RenderWindow& window) const {
    sf::Vector2i m = sf::Mouse::getPosition(window);
    return sprite.getGlobalBounds().contains(static_cast<float>(m.x), static_cast<float>(m.y));
}

void Button::draw(sf::RenderWindow& window) const { window.draw(sprite); }
sf::FloatRect Button::getBounds() const { return sprite.getGlobalBounds(); }