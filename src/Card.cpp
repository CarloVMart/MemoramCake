#include "Card.hpp"

Card::Card() : id(-1), revealed(false), matchedFlag(false) {}

void Card::setFrontTexture(const sf::Texture& tex) { sFront.setTexture(tex); }
void Card::setBackTexture(const sf::Texture& tex) { sBack.setTexture(tex); }
void Card::setId(int i) { id = i; }
int Card::getId() const { return id; }

void Card::reveal() { revealed = true; }
void Card::hide() { revealed = false; }
bool Card::isFaceUp() const { return revealed; }
bool Card::isMatched() const { return matchedFlag; }
void Card::setMatched(bool m) { matchedFlag = m; }

sf::Sprite& Card::frontSprite() { return sFront; }
sf::Sprite& Card::backSprite() { return sBack; }