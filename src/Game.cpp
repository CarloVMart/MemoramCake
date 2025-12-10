#include "Game.hpp"
#include <iostream>

Game::Game() 
    : window(sf::VideoMode(1920,1080), "MemoramCake"),
      windowedSize(1920,1080),
      isFullscreen(false),
      state(MENU),
      paused(false)
{
    // background
    if (!backgroundTexture.loadFromFile("assets/images/fondo.png"))
        std::cerr << "Error cargando fondo.png\n";
    background.setTexture(backgroundTexture);
    scaleBackground();

    // play button
    if (!playTexture.loadFromFile("assets/images/play_button.png"))
        std::cerr << "Error cargando play_button.png\n";
    playButton = new Button(playTexture);
    centerPlayButton();

    // font
    if (!font.loadFromFile("assets/fonts/LuckiestGuy-Regular.ttf"))
        std::cerr << "Error cargando fuente\n";

    // board
    board.loadAssets();
    board.initCards();
    timer.reset(0.f); // MENU inicia con tiempo 0
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
            if (ev.key.code == sf::Keyboard::Escape && state == PLAYING)
                paused = !paused;
        }
        if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            if (state == MENU) {
                if (playButton->isClicked(window)) {
                    state = PLAYING;
                    timer.reset(60.f);
                    board.initCards();
                }
            } else if (state == PLAYING && !paused) {
                board.handleClick(mousePos);
            }
        }
    }
}

void Game::update(float dt) {
    timer.update(dt);
    board.update(dt);

    if (state == PLAYING && (timer.getTime() <= 0.f || board.matchedPairs() == 15)) {
        state = MENU;
        timer.reset(0.f);
        board.revealAll();
    }
}

void Game::render() {
    window.clear();
    window.draw(background);

    // Obtener layout del grid de cartas
    float cardW, cardH, spacingX, spacingY, startX, startY;
    board.computeLayout(cardW, cardH, spacingX, spacingY, startX, startY);
    float totalWidth = board.getCols() * cardW + (board.getCols() - 1) * spacingX;
    float totalHeight = board.getRows() * cardH + (board.getRows() - 1) * spacingY;
    float gridCenterY = startY + totalHeight / 2.f;

    // --- TIEMPO a la izquierda ---
sf::Text textTiempo("TIEMPO", font, 130);
textTiempo.setFillColor(sf::Color::White);
textTiempo.setOrigin(textTiempo.getLocalBounds().width/2, textTiempo.getLocalBounds().height/2);
textTiempo.setPosition(startX / 2.f, gridCenterY - 406.f + 50.f); // bajar 200px
window.draw(textTiempo);

sf::Text valueTiempo(std::to_string(int(timer.getTime())), font, 130);
valueTiempo.setFillColor(sf::Color::White);
valueTiempo.setOrigin(valueTiempo.getLocalBounds().width/2, valueTiempo.getLocalBounds().height/2);
valueTiempo.setPosition(startX / 2.f, gridCenterY - 406.f + 160.f); // bajar 100px más que el título
window.draw(valueTiempo);

// --- PARES a la derecha ---
float rightX = startX + totalWidth + (1920.f - (startX + totalWidth)) / 2.f;
sf::Text textPares("PARES", font, 130);
textPares.setFillColor(sf::Color::White);
textPares.setOrigin(textPares.getLocalBounds().width/2, textPares.getLocalBounds().height/2);
textPares.setPosition(rightX, gridCenterY - 406.f + 50.f); // bajar 200px
window.draw(textPares);

sf::Text valuePares(std::to_string(board.matchedPairs()), font, 130);
valuePares.setFillColor(sf::Color::White);
valuePares.setOrigin(valuePares.getLocalBounds().width/2, valuePares.getLocalBounds().height/2);
valuePares.setPosition(rightX, gridCenterY - 406.f + 160.f); // bajar 100px más que el título
window.draw(valuePares);


    // Dibujar cartas
    board.draw(window);

    // Dibujar playButton solo en MENU
    if (state == MENU) playButton->draw(window);

    window.display();
}

void Game::toggleFullscreen() {
    isFullscreen = !isFullscreen;
    if (isFullscreen)
        window.create(sf::VideoMode::getDesktopMode(), "MemoramCake", sf::Style::Fullscreen);
    else
        window.create(sf::VideoMode(windowedSize.x, windowedSize.y), "MemoramCake");
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