#pragma once
#include <SFML/Graphics.hpp>

class Card {
public:
    int id;
    bool flipped = false;
    bool matched = false;
    sf::Sprite front;
    sf::Sprite back;

    void flip() { flipped = !flipped; }
    bool contains(sf::Vector2f pos) {
        return front.getGlobalBounds().contains(pos);
    }
    void draw(sf::RenderWindow& window) {
        if (flipped || matched)
            window.draw(front);
        else
            window.draw(back);
    }
};