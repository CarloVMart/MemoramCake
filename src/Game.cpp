#include "Game.h"
#include <iostream>

Game::Game() {
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    window.create(desktop, "Memorama", sf::Style::Fullscreen);
    windowedSize = {1920, 1080};
    isFullscreen = true;

    state = MENU;
    paused = false;

    if (!backgroundTexture.loadFromFile("assets/images/fondo.png"))
        std::cerr << "Error cargando fondo.png" << std::endl;

    background.setTexture(backgroundTexture);
    float scaleX = float(desktop.width) / backgroundTexture.getSize().x;
    float scaleY = float(desktop.height) / backgroundTexture.getSize().y;
    background.setScale(scaleX, scaleY);

    if (!playTexture.loadFromFile("assets/images/play_button.png"))
        std::cerr << "Error cargando play_button.png" << std::endl;

    playButton = new Button(playTexture);
    playButton->sprite.setPosition(
        desktop.width / 2.f - playButton->sprite.getGlobalBounds().width / 2.f,
        desktop.height / 2.f - playButton->sprite.getGlobalBounds().height / 2.f
    );

    if (!timerFont.loadFromFile("assets/fonts/LuckiestGuy-Regular.ttf"))
        std::cerr << "Error cargando fuente" << std::endl;

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
        update(dt);
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape)
                paused = !paused;
            if (event.key.code == sf::Keyboard::F11)
                toggleFullscreen();
        }

        if (state == MENU && event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            if (playButton->contains(mousePos)) {
                state = PLAYING;
                timer.reset(60.f);
                board.initCards();
            }
        }

        if (state == PLAYING && event.type == sf::Event::MouseButtonPressed && !paused) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            board.handleClick(mousePos);
        }
    }
}

void Game::update(float dt) {
    if (paused || state != PLAYING) return;

    timer.update(dt);
    board.update(dt);

    if (timer.getTime() <= 0.f || board.matchedPairs() == 15)
        state = MENU;
}

void Game::render() {
    window.clear();
    window.draw(background);

    if (state == MENU)
        window.draw(playButton->sprite);
    else if (state == PLAYING) {
        board.draw(window);

        sf::Text timerText;
        timerText.setFont(timerFont);
        timerText.setCharacterSize(40);
        timerText.setFillColor(sf::Color::White);
        timerText.setString("Tiempo: " + std::to_string(int(timer.getTime())));
        timerText.setPosition(20.f, 20.f);
        window.draw(timerText);

        sf::Text pairText;
        pairText.setFont(timerFont);
        pairText.setCharacterSize(40);
        pairText.setFillColor(sf::Color::White);
        pairText.setString("Pares: " + std::to_string(board.matchedPairs()));
        pairText.setPosition(20.f, 70.f);
        window.draw(pairText);
    }

    window.display();
}

void Game::toggleFullscreen() {
    isFullscreen = !isFullscreen;

    sf::Vector2u size;
    sf::Uint32 style;

    if (isFullscreen) {
        sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
        size = {desktop.width, desktop.height};
        style = sf::Style::Fullscreen;
    } else {
        size = windowedSize;
        style = sf::Style::Titlebar | sf::Style::Resize;
    }

    window.close();
    window.create(sf::VideoMode(size.x, size.y), "MemoramCake", style);
    window.setFramerateLimit(60);
}