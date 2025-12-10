#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Card.hpp"

class Board {
private:
    Card cards[30];
    sf::Texture textures[15];
    sf::Texture backTexture;
    Card* first;
    Card* second;

    // bloqueo mientras se espera a resolver par
    bool waiting;
    float waitTimer; // segundos

public:
    Board();
    void loadAssets();
    void initCards();
    void update(float dt);
    void draw(sf::RenderWindow& window);
    void handleClick(sf::Vector2f pos);
    int matchedPairs();
};