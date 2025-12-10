#pragma once
#include <SFML/Graphics.hpp>
#include "Board.hpp"
#include "Button.hpp"
#include "Timer.hpp"

class Game {
private:
    sf::RenderWindow window;
    sf::Vector2u windowedSize;
    bool isFullscreen;

    enum State { MENU, PLAYING };
    State state;

    bool paused;

    sf::Texture backgroundTexture;
    sf::Sprite background;

    sf::Texture playTexture;
    Button* playButton;

    sf::Font timerFont;

    Board board;
    Timer timer;

public:
    Game();
    ~Game();
    void run();

private:
    void processEvents();
    void update(float dt);
    void render();
    void toggleFullscreen();
};