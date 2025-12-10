#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include <vector>
#include "Card.hpp"

class Board {
public:
    Board();

    // load assets (textures) — call once
    void loadAssets();

    // create/shuffle cards — call at start or on restart
    void initCards();

    // update logic (dt in seconds)
    void update(float dt);

    // draw all cards (will compute scaling & positions based on window size)
    void draw(sf::RenderWindow& window);

    // handle click: pass mouse position in window coords
    void handleClick(sf::Vector2f pos);

    // number of pairs matched
    int matchedPairs() const;

private:
    std::array<sf::Texture, 15> textures;
    sf::Texture backTexture;
    std::array<Card, 30> cards;

    // indices for comparison (-1 means none)
    int firstIndex;
    int secondIndex;

    // when two are flipped, wait this many seconds then resolve
    float resolveTimer;
    bool resolving;

    // layout params (cols/rows)
    const int cols = 6;
    const int rows = 5;

    // helpers
    void computeLayout(const sf::RenderWindow& window, float& outCardW, float& outCardH,
                       float& outSpacingX, float& outSpacingY, float& outStartX, float& outStartY);
};