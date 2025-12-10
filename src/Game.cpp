#include "Game.hpp"
#include <iostream>

Game::Game()
: window(sf::VideoMode(1920,1080), "MemoramCake")
, windowedSize(1920,1080)
, isFullscreen(false)
, state(MENU)
, paused(false)
{
    // background
    if (!backgroundTexture.loadFromFile("assets/images/fondo.png")) {
        std::cerr << "Error cargando fondo.png\n";
    }
    background.setTexture(backgroundTexture);
    scaleBackground();

    // play button
    if (!playTexture.loadFromFile("assets/images/play_button.png")) {
        std::cerr << "Error cargando play_button.png\n";
    }
    playButton = new Button(playTexture);
    centerPlayButton();

    // font
    if (!font.loadFromFile("assets/fonts/LuckiestGuy-Regular.ttf")) {
        std::cerr << "Error cargando fuente\n";
    }

    // load board assets & init
    board.loadAssets();
    board.initCards();

    timer.reset(60.f);
}

Game::~Game() { delete playButton; }

void Game::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        processEvents();
        if (state == PLAYING && !paused) update(dt);
        render();
    }
}

void Game::processEvents() {
    sf::Event ev;
    while (window.pollEvent(ev)) {
        if (ev.type == sf::Event::Closed) window.close();

        if (ev.type == sf::Event::KeyPressed) {
            if (ev.key.code == sf::Keyboard::F11) toggleFullscreen();
            if (ev.key.code == sf::Keyboard::Escape) {
                if (state == PLAYING) paused = !paused;
            }
        }

        if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            if (state == MENU) {
                if (playButton->isClicked(window)) {
                    // start game
                    state = PLAYING;
                    timer.reset(60.f);
                    board.initCards();
                }
            } else if (state == PLAYING) {
                if (!paused)
                    board.handleClick(mousePos);
            }
        }
    }
}

void Game::update(float dt) {
    timer.update(dt);
    board.update(dt);

    if (timer.getTime() <= 0.f || board.matchedPairs() == 15) {
        state = MENU;
        // leave cards as they are; user can press play to reset
    }
}

void Game::render() {
    window.clear();
    window.draw(background);

    if (state == MENU) {
        playButton->draw(window);
    } else if (state == PLAYING) {
        board.draw(window);

        // Time (left) at left-top area
        sf::Text t;
        t.setFont(font);
        t.setCharacterSize(48);
        t.setFillColor(sf::Color::White);
        t.setString("TIEMPO: " + std::to_string(int(timer.getTime())));
        t.setPosition(60.f, 40.f);
        window.draw(t);

        // Pairs at right-top
        sf::Text p;
        p.setFont(font);
        p.setCharacterSize(48);
        p.setFillColor(sf::Color::White);
        p.setString("PARES: " + std::to_string(board.matchedPairs()));
        p.setPosition(window.getSize().x - 320.f, 40.f);
        window.draw(p);
    }

    window.display();
}

void Game::toggleFullscreen() {
    isFullscreen = !isFullscreen;
    if (isFullscreen) {
        window.create(sf::VideoMode::getDesktopMode(), "MemoramCake", sf::Style::Fullscreen);
    } else {
        window.create(sf::VideoMode(windowedSize.x, windowedSize.y), "MemoramCake");
    }
    centerPlayButton();
    scaleBackground();
}

void Game::centerPlayButton() {
    auto bounds = playButton->getBounds();
    float x = window.getSize().x / 2.f - bounds.width / 2.f;
    float y = window.getSize().y / 2.f - bounds.height / 2.f;
    playButton->setPosition(x, y);
}

void Game::scaleBackground() {
    if (backgroundTexture.getSize().x == 0) return;
    background.setScale(
        float(window.getSize().x) / backgroundTexture.getSize().x,
        float(window.getSize().y) / backgroundTexture.getSize().y
    );
}