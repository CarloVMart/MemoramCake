#include "Board.hpp"
#include <algorithm>
#include <random>
#include <chrono>
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>

Board::Board() : firstIndex(-1), secondIndex(-1), resolveTimer(0.f), resolving(false) {}

void Board::loadAssets() {
    for (int i = 0; i < 15; ++i) {
        std::ostringstream ss;
        ss << std::setw(2) << std::setfill('0') << (i+1);
        std::string filename = "assets/images/" + ss.str() + ".png";
        if (!textures[i].loadFromFile(filename)) {
            std::cerr << "Failed to load " << filename << "\n";
        }
    }
    if (!backTexture.loadFromFile("assets/images/back.png")) {
        std::cerr << "Failed to load back.png\n";
    }
}

void Board::initCards() {
    std::vector<int> ids;
    ids.reserve(30);
    for (int i = 0; i < 15; ++i) {
        ids.push_back(i);
        ids.push_back(i);
    }
    unsigned seed = (unsigned)std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(ids.begin(), ids.end(), std::default_random_engine(seed));

    for (int i = 0; i < 30; ++i) {
        cards[i].setId(ids[i]);
        cards[i].setFrontTexture(textures[ids[i]]);
        cards[i].setBackTexture(backTexture);
        cards[i].hide();
        cards[i].setMatched(false);
    }
    firstIndex = secondIndex = -1;
    resolving = false;
    resolveTimer = 0.f;
}

void Board::computeLayout(float& outCardW, float& outCardH,
                          float& outSpacingX, float& outSpacingY,
                          float& outStartX, float& outStartY) {
    outCardW = 122.f;
    outCardH = 180.f;
    outSpacingX = 20.f;
    outSpacingY = 20.f;

    float totalWidth = cols * outCardW + (cols - 1) * outSpacingX;
    float totalHeight = rows * outCardH + (rows - 1) * outSpacingY;

    outStartX = (1920.f - totalWidth) / 2.f;
    outStartY = (1080.f - totalHeight) / 2.f;
}

void Board::update(float dt) {
    if (resolving) {
        resolveTimer += dt;
        if (resolveTimer >= 0.55f) {
            if (firstIndex >= 0 && secondIndex >= 0) {
                if (cards[firstIndex].getId() == cards[secondIndex].getId()) {
                    cards[firstIndex].setMatched(true);
                    cards[secondIndex].setMatched(true);
                } else {
                    cards[firstIndex].hide();
                    cards[secondIndex].hide();
                }
            }
            firstIndex = secondIndex = -1;
            resolving = false;
            resolveTimer = 0.f;
        }
    }
}

void Board::draw(sf::RenderWindow& window) {
    float cardW, cardH, spacingX, spacingY, startX, startY;
    computeLayout(cardW, cardH, spacingX, spacingY, startX, startY);

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            int idx = r * cols + c;
            Card& card = cards[idx];
            sf::Sprite& sFront = card.frontSprite();
            sf::Sprite& sBack = card.backSprite();

            if (sBack.getTexture()) {
                sf::Vector2u texSz = sBack.getTexture()->getSize();
                float sx = cardW / float(texSz.x);
                float sy = cardH / float(texSz.y);
                sBack.setScale(sx, sy);
                sFront.setScale(sx, sy);
            }

            float x = startX + c * (cardW + spacingX);
            float y = startY + r * (cardH + spacingY);
            sBack.setPosition(x, y);
            sFront.setPosition(x, y);

            if (card.isFaceUp() || card.isMatched()) window.draw(sFront);
            else window.draw(sBack);
        }
    }
}

void Board::handleClick(sf::Vector2f pos) {
    if (resolving) return;
    for (int i = 0; i < 30; ++i) {
        Card& card = cards[i];
        if (card.isMatched() || card.isFaceUp()) continue;
        if (card.backSprite().getGlobalBounds().contains(pos)) {
            card.reveal();
            if (firstIndex == -1) firstIndex = i;
            else if (secondIndex == -1) {
                secondIndex = i;
                resolving = true;
                resolveTimer = 0.f;
            }
            break;
        }
    }
}

int Board::matchedPairs() const {
    int count = 0;
    for (const auto& c : cards) if (c.isMatched()) ++count;
    return count / 2;
}

void Board::revealAll() {
    for (auto& c : cards) c.reveal();
}