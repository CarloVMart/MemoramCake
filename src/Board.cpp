#include "Board.hpp"
#include <algorithm>
#include <random>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <iostream>

Board::Board() : first(nullptr), second(nullptr), waiting(false), waitTimer(0.f) {}

// Carga texturas con nombres "01.png", "02.png", ... "15.png"
void Board::loadAssets() {
    for (int i = 0; i < 15; i++) {
        std::ostringstream ss;
        ss << std::setw(2) << std::setfill('0') << (i + 1); // 01 .. 15
        std::string filename = "assets/images/" + ss.str() + ".png";
        if (!textures[i].loadFromFile(filename)) {
            std::cerr << "Failed to load image \"" << filename << "\". Reason: Unable to open file\n";
        }
    }

    if (!backTexture.loadFromFile("assets/images/back.png")) {
        std::cerr << "Failed to load image \"assets/images/back.png\". Reason: Unable to open file\n";
    }
}

void Board::initCards() {
    // reset estado
    first = nullptr;
    second = nullptr;
    waiting = false;
    waitTimer = 0.f;

    // generar ids 0,0,1,1,...14,14
    std::vector<int> ids;
    ids.reserve(30);
    for (int i = 0; i < 15; ++i) {
        ids.push_back(i);
        ids.push_back(i);
    }

    // mezclar con mt19937
    std::mt19937 rng((unsigned)std::chrono::system_clock::now().time_since_epoch().count());
    std::shuffle(ids.begin(), ids.end(), rng);

    // asignar texturas a cada carta, y ponerlas ocultas
    for (int i = 0; i < 30; ++i) {
        cards[i].setFront(textures[ids[i]]);
        cards[i].setBack(backTexture);
        cards[i].setId(ids[i]);
        cards[i].hide();        // asegurarse que estén boca abajo
        cards[i].setMatched(false);
    }
}

void Board::update(float dt) {
    if (waiting) {
        waitTimer += dt;
        // cuando el tiempo de espera pasa, resolver el par
        if (waitTimer >= 0.6f) {
            if (first && second) {
                if (first->getId() == second->getId()) {
                    first->setMatched(true);
                    second->setMatched(true);
                    // dejarlas visibles (matched)
                } else {
                    // ocultarlas
                    first->hide();
                    second->hide();
                }
            }
            // reset
            first = nullptr;
            second = nullptr;
            waiting = false;
            waitTimer = 0.f;
        }
    }
    // si no waiting, no hacer nada especial aquí (las cartas no necesitan animación adicional)
}

void Board::draw(sf::RenderWindow& window) {
    sf::Vector2u size = window.getSize();

    int rows = 5;
    int cols = 6;

    // tamaño base de carta en px (tu original 122x180)
    float cardW = 122.f;
    float cardH = 180.f;

    // escalar para que quepan en la ventana (usar un 80% del área para márgenes)
    float scaleW = (size.x * 0.8f) / (cols * cardW);
    float scaleH = (size.y * 0.8f) / (rows * cardH);
    float scale = std::min(scaleW, scaleH);

    // spacing en px relativo al scale (por ejemplo 15 px base)
    float spacingBase = 15.f;
    float spacing = spacingBase * scale;

    // calcular ancho/alto total ocupado (sin añadir spacing extra a la derecha/bajo)
    float totalW = cols * (cardW * scale) + (cols - 1) * spacing;
    float totalH = rows * (cardH * scale) + (rows - 1) * spacing;

    float startX = (size.x - totalW) / 2.f;
    float startY = (size.y - totalH) / 2.f;

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            int index = r * cols + c;

            sf::Sprite& sprite = cards[index].getSprite();
            sprite.setScale(scale, scale);
            sprite.setPosition(
                startX + c * (cardW * scale + spacing),
                startY + r * (cardH * scale + spacing)
            );

            window.draw(sprite);
        }
    }
}

void Board::handleClick(sf::Vector2f pos) {
    // si ya hay dos cartas descubiertas y estamos esperando, ignorar clicks
    if (waiting) return;

    for (int i = 0; i < 30; ++i) {
        sf::Sprite& s = cards[i].getSprite();
        if (s.getGlobalBounds().contains(pos)) {
            if (cards[i].isMatched()) return;
            if (cards[i].isFaceUp()) return;

            // revelar carta
            cards[i].reveal();

            if (!first) {
                first = &cards[i];
            } else if (!second) {
                second = &cards[i];
                // ahora que hay dos cartas, iniciar el delay para resolver
                waiting = true;
                waitTimer = 0.f;
            }
            return;
        }
    }
}

int Board::matchedPairs() {
    int count = 0;
    for (int i = 0; i < 30; ++i) {
        if (cards[i].isMatched()) count++;
    }
    return count / 2;
}