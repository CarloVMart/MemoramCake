#pragma once
#include <SFML/Graphics.hpp>
#include "Board.h"
#include "Button.h"
#include "Timer.h"

enum GameState { MENU, PLAYING };

class Game {
public:
    Game();
    ~Game();
    void run();

private:
    sf::RenderWindow window;
    bool isFullscreen;
    sf::Vector2u windowedSize;

    GameState state;
    bool paused = false;

    // Fondo
    sf::Texture backgroundTexture;
    sf::Sprite background;

    // Botón Play
    Button* playButton;
    sf::Texture playTexture;

    // Tablero
    Board board;

    // Timer
    Timer timer;
    sf::Font timerFont;

    // Métodos internos
    void processEvents();
    void update(float dt);
    void render();
    void toggleFullscreen();
};