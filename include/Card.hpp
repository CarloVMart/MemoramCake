#pragma once
#include <SFML/Graphics.hpp>

class Card {
public:
    Card();

    void setFrontTexture(const sf::Texture& tex);
    void setBackTexture(const sf::Texture& tex);
    void setId(int i);
    int getId() const;

    void reveal();
    void hide();
    bool isFaceUp() const;
    bool isMatched() const;
    void setMatched(bool m);

    sf::Sprite& frontSprite();
    sf::Sprite& backSprite();

private:
    int id;
    bool revealed;
    bool matchedFlag;
    sf::Sprite sFront;
    sf::Sprite sBack;
};