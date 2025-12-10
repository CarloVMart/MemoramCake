#pragma once
#include <SFML/Graphics.hpp>
#include "Board.hpp"
#include "Button.hpp"
#include "Timer.hpp"

class Game {
public:
    Game();
    ~Game();
    void run();

private:
    sf::RenderWindow window;
    sf::Vector2u windowedSize;
    bool isFullscreen;

    enum State { MENU, PLAYING };
    State state;
    bool paused;

    // UI / assets
    sf::Texture backgroundTexture;
    sf::Sprite background;

    sf::Texture playTexture;
    Button* playButton;

    sf::Font font;

    // game
    Board board;
    Timer timer;

    // helpers
    void processEvents();
    void update(float dt);
    void render();
    void toggleFullscreen();
    void centerPlayButton();
    void scaleBackground();
};