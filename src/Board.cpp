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
        // filenames: 01.png .. 09.png .. 15.png
        std::ostringstream ss;
        ss << std::setw(2) << std::setfill('0') << (i+1);
        std::string filename = std::string("assets/images/") + ss.str() + ".png";
        if (!textures[i].loadFromFile(filename)) {
            std::cerr << "Failed to load image \"" << filename << "\". Reason: Unable to open file\n";
        }
    }
    if (!backTexture.loadFromFile("assets/images/back.png")) {
        std::cerr << "Failed to load image \"assets/images/back.png\". Reason: Unable to open file\n";
    }
}

void Board::initCards() {
    // prepare ids 0..14 duplicated
    std::vector<int> ids;
    ids.reserve(30);
    for (int i = 0; i < 15; ++i) { ids.push_back(i); ids.push_back(i); }

    // shuffle
    unsigned seed = (unsigned)std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(ids.begin(), ids.end(), std::default_random_engine(seed));

    // assign textures & reset states
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

void Board::computeLayout(const sf::RenderWindow& window, float& outCardW, float& outCardH,
                          float& outSpacingX, float& outSpacingY, float& outStartX, float& outStartY)
{
    // Original art size (your PNGs): 122 x 180
    const float origW = 122.f;
    const float origH = 180.f;
    const float ratio = origW / origH;

    // target area uses 85% of window to leave margins
    float areaW = window.getSize().x * 0.85f;
    float areaH = window.getSize().y * 0.85f;

    // compute candidate sizes so grid fits
    float candidateW = areaW / (cols + 0.5f);   // rough
    float candidateH = areaH / (rows + 0.5f);

    // keep aspect ratio of artwork
    outCardW = candidateW;
    outCardH = outCardW / ratio;
    if (outCardH > candidateH) { outCardH = candidateH; outCardW = outCardH * ratio; }

    // spacing: small gap relative to card width, but at least 8 px
    outSpacingX = std::max(8.f, (window.getSize().x - cols * outCardW) / (cols + 1));
    outSpacingY = std::max(8.f, (window.getSize().y - rows * outCardH) / (rows + 1));

    // start positions to center grid
    float totalW = cols * outCardW + (cols - 1) * outSpacingX;
    float totalH = rows * outCardH + (rows - 1) * outSpacingY;
    outStartX = (window.getSize().x - totalW) / 2.f;
    outStartY = (window.getSize().y - totalH) / 2.f;
}

void Board::update(float dt) {
    if (resolving) {
        resolveTimer += dt;
        if (resolveTimer >= 0.55f) { // small delay then resolve
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
    computeLayout(window, cardW, cardH, spacingX, spacingY, startX, startY);

    // set sprite transforms and draw
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            int idx = r * cols + c;
            Card& card = cards[idx];

            // choose sprite based on revealed/matched
            sf::Sprite& sFront = card.frontSprite();
            sf::Sprite& sBack = card.backSprite();

            // scale relative to original texture size (assume textures exist)
            // use back sprite's texture size as baseline (they share original ratio)
            if (sBack.getTexture()) {
                sf::Vector2u texSz = sBack.getTexture()->getSize();
                float sx = cardW / float(texSz.x);
                float sy = cardH / float(texSz.y);
                sBack.setScale(sx, sy);
                sFront.setScale(sx, sy);
            } else {
                sBack.setScale(1.f,1.f);
                sFront.setScale(1.f,1.f);
            }

            float x = startX + c * (cardW + spacingX);
            float y = startY + r * (cardH + spacingY);

            sBack.setPosition(x, y);
            sFront.setPosition(x, y);

            // draw appropriate sprite
            if (card.isFaceUp() || card.isMatched())
                window.draw(sFront);
            else
                window.draw(sBack);
        }
    }
}

void Board::handleClick(sf::Vector2f pos) {
    if (resolving) return; // ignore clicks while resolving

    // need a temporary window (we can't compute Window here) - but sprites have global bounds updated in draw()
    // We'll iterate and check bounds of backSprite (position/scale updated in last draw call)
    for (int i = 0; i < 30; ++i) {
        Card& card = cards[i];
        if (card.isMatched() || card.isFaceUp()) continue;

        if (card.backSprite().getGlobalBounds().contains(pos)) {
            // reveal it
            card.reveal();

            if (firstIndex == -1) firstIndex = i;
            else if (secondIndex == -1) {
                secondIndex = i;
                // start resolving timer (block further clicks)
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