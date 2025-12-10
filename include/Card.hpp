#pragma once
#include <SFML/Graphics.hpp>

class Card {
private:
    sf::Texture* frontTexture;
    sf::Texture* backTexture;
    sf::Sprite sprite;

    int id;
    bool faceUp;
    bool matched;

public:
    Card();

    void setFront(sf::Texture& tex);
    void setBack(sf::Texture& tex);
    void setId(int i);
    int getId() const;

    void reveal();
    void hide();

    bool isFaceUp() const;
    bool isMatched() const;
    void setMatched(bool m);

    sf::Sprite& getSprite();

    void setPosition(float x, float y);
};