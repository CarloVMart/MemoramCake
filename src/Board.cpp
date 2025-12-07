#include "Board.h"
#include <algorithm>
#include <random>
#include <chrono>
#include <string>
#include <iostream>

Board::Board() : first(nullptr), second(nullptr) {}

void Board::loadAssets() {
    for (int i = 0; i < 15; i++) {
        std::string filename = std::string("assets/images/") + (i < 9 ? "0" : "") + std::to_string(i+1) + ".png";
        if (!textures[i].loadFromFile(filename)) {
            std::cerr << "Error cargando la carta: " << filename << std::endl;
        }
    }

    if (!backTexture.loadFromFile("assets/images/back.png")) {
        std::cerr << "Error cargando back.png" << std::endl;
    }
}

void Board::initCards() {
    first = nullptr;
    second = nullptr;

    std::vector<int> ids;
    for (int i = 0; i < 15; i++) {
        ids.push_back(i);
        ids.push_back(i);
    }

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(ids.begin(), ids.end(), std::default_random_engine(seed));

    int cols = 6;
    int rows = 5;

    float originalWidth = 122.f;
    float originalHeight = 180.f;
    float ratio = originalWidth / originalHeight;

    float maxCardWidth = 1920.f / (cols + 0.5f);
    float maxCardHeight = 1080.f / (rows + 0.5f);

    float cardWidth = maxCardWidth;
    float cardHeight = cardWidth / ratio;

    if (cardHeight > maxCardHeight) {
        cardHeight = maxCardHeight;
        cardWidth = cardHeight * ratio;
    }

    float totalCardsWidth = cols * cardWidth;
    float spacingX = (1920 - totalCardsWidth) / (cols + 1);
    if (spacingX < 10.f) spacingX = 10.f;

    float startX = (1920 - (totalCardsWidth + spacingX * (cols - 1))) / 2.f;
    float totalCardsHeight = rows * cardHeight;
    float spacingY = (1080 - totalCardsHeight) / (rows + 1);
    if (spacingY < 10.f) spacingY = 10.f;
    float startY = spacingY;

    for (int i = 0; i < 30; i++) {
        Card c;
        c.id = ids[i];

        c.front.setTexture(textures[ids[i]]);
        c.front.setScale(cardWidth / originalWidth, cardHeight / originalHeight);

        c.back.setTexture(backTexture);
        c.back.setScale(cardWidth / originalWidth, cardHeight / originalHeight);

        int col = i % cols;
        int row = i / cols;

        float posX = startX + col * (cardWidth + spacingX);
        float posY = startY + row * (cardHeight + spacingY);

        c.front.setPosition(posX, posY);
        c.back.setPosition(posX, posY);

        cards[i] = c;
    }
}

void Board::update(float dt) {
    if (first && second) {
        if (first->id == second->id) {
            first->matched = true;
            second->matched = true;
        } else {
            first->flipped = false;
            second->flipped = false;
        }
        first = nullptr;
        second = nullptr;
    }
}

void Board::draw(sf::RenderWindow& window) {
    for (int i = 0; i < 30; i++) {
        cards[i].draw(window);
    }
}

void Board::handleClick(sf::Vector2f pos) {
    for (int i = 0; i < 30; i++) {
        Card& c = cards[i];
        if (!c.flipped && !c.matched && c.contains(pos)) {
            c.flip();
            if (!first)
                first = &c;
            else if (!second)
                second = &c;
            break;
        }
    }
}

int Board::matchedPairs() {
    int count = 0;
    for (int i = 0; i < 30; i++)
        if (cards[i].matched) count++;
    return count / 2;
}