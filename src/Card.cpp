#include "Card.hpp"

Card::Card() {
    frontTexture = nullptr;
    backTexture = nullptr;
    id = -1;
    faceUp = false;
    matched = false;
}

void Card::setFront(sf::Texture& tex) {
    frontTexture = &tex;
    if (faceUp)
        sprite.setTexture(*frontTexture);
}

void Card::setBack(sf::Texture& tex) {
    backTexture = &tex;
    if (!faceUp)
        sprite.setTexture(*backTexture);
}

void Card::setId(int i) {
    id = i;
}

int Card::getId() const {
    return id;
}

void Card::reveal() {
    faceUp = true;
    if (frontTexture)
        sprite.setTexture(*frontTexture);
}

void Card::hide() {
    faceUp = false;
    if (backTexture)
        sprite.setTexture(*backTexture);
}

bool Card::isFaceUp() const {
    return faceUp;
}

bool Card::isMatched() const {
    return matched;
}

void Card::setMatched(bool m) {
    matched = m;
}

sf::Sprite& Card::getSprite() {
    return sprite;
}

void Card::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}