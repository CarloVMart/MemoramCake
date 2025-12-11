#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include <vector>
#include <functional>
#include "Card.hpp"

class Board {
public:
    Board();
    void loadAssets();
    void initCards();
    void update(float dt);
    void draw(sf::RenderWindow& window);
    void handleClick(sf::Vector2f pos);
    int matchedPairs() const;
    void revealAll();
    int getCols() const { return cols; }
    int getRows() const { return rows; }

    std::function<void()> onFlipSound;

    void computeLayout(float& outCardW, float& outCardH,
                       float& outSpacingX, float& outSpacingY,
                       float& outStartX, float& outStartY);
private:
    std::array<sf::Texture, 15> textures;
    sf::Texture backTexture;
    std::array<Card, 30> cards;
    int firstIndex;
    int secondIndex;
    float resolveTimer;
    bool resolving;
    const int cols = 6;
    const int rows = 5;
};