#include "../Headers/GameLogic.h"

#include "../Headers/SplashScreen.h"
void Game::toggleFullscreen() {
    isFullscreen = !isFullscreen;

    if (isFullscreen) {
        window.create(sf::VideoMode::getDesktopMode(), "Game", sf::State::Fullscreen);
        window.setFramerateLimit(30);
    } else {
        window.create(sf::VideoMode({640, 480}), "Game", sf::State::Windowed);
        window.setFramerateLimit(30);
    }

    updateView();
}
void Game::updateView() {
    const sf::Vector2u winSize = window.getSize();
    sf::Vector2f targetSize(640.f, 480.f);

    const float windowRatio = static_cast<float>(winSize.x) / winSize.y;
    const float targetRatio = targetSize.x / targetSize.y;

    sf::View view(sf::FloatRect({0, 0}, {targetSize.x, targetSize.y}));

    if (windowRatio > targetRatio) {
        const float newWidth = targetRatio * winSize.y;
        view.setViewport(sf::FloatRect({(1.f - newWidth / winSize.x) / 2.f, 0.f}, {newWidth / winSize.x, 1.f}));
    } else {
        const float newHeight = winSize.x / targetRatio;
        view.setViewport(sf::FloatRect({0.f, (1.f - newHeight / winSize.y) / 2.f},{ 1.f, newHeight / winSize.y}));
    }

    window.setView(view);
}



void Game::playMusBattle1() {
    if (!mus_battle1.openFromFile("./mus/mus_battle1.ogg")) {
        std::cerr << "Failed to load mus_battle1.ogg file" << std::endl;
        throw std::runtime_error("Failed to load mus_battle1.ogg file");
    }
    mus_battle1.setLooping(true);
    mus_battle1.play();
}

void Game::centerPlayer() {
    player.setOrigin({player.getGlobalBounds().size.x/2.f,player.getGlobalBounds().size.y/2.f});
    player.setPosition(battlebox.getCenter());
    player.setOrigin({0,0});
}

void Game::handleEvents() {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        if (event->is<sf::Event::KeyPressed>()) {
            const auto* keyPressed = event->getIf<sf::Event::KeyPressed>();
            keysPressed.insert(keyPressed->scancode);

            if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                window.close();
            }
            if (keyPressed->scancode == sf::Keyboard::Scancode::F4) {
                toggleFullscreen();
            }
        }
        if (event->is<sf::Event::KeyReleased>()) {
            const auto* keyReleased = event->getIf<sf::Event::KeyReleased>();
            keysPressed.erase(keyReleased->scancode);
        }
    }
}

sf::Vector2f Game::calculateMoveOffset() const {
    constexpr float speed = 4.0f;
    sf::Vector2f moveOffset(0, 0);

    if (keysPressed.contains(sf::Keyboard::Scancode::Up) || keysPressed.contains(sf::Keyboard::Scancode::W)) {
        moveOffset.y -= speed;
    }
    if (keysPressed.contains(sf::Keyboard::Scancode::Down) || keysPressed.contains(sf::Keyboard::Scancode::S)) {
        moveOffset.y += speed;
    }
    if (keysPressed.contains(sf::Keyboard::Scancode::Left) || keysPressed.contains(sf::Keyboard::Scancode::A)){
        moveOffset.x -= speed;
    }
    if (keysPressed.contains(sf::Keyboard::Scancode::Right) || keysPressed.contains(sf::Keyboard::Scancode::D)){
        moveOffset.x += speed;
    }

    return moveOffset;
}

void Game::enforceBattleBoxBounds(sf::Vector2f &moveOffset) const {
    const sf::FloatRect innerBounds=battlebox.getInnerBounds();

    const sf::Vector2f playerPos = player.getPosition() + moveOffset;
    const sf::Vector2f playerSize = {player.getGlobalBounds().size.x, player.getGlobalBounds().size.y};

    if (playerPos.x < innerBounds.position.x) {
        moveOffset.x = innerBounds.position.x - player.getPosition().x - battlebox.getOutlineThickness() + 1;
    }
    if (playerPos.y < innerBounds.position.y) {
        moveOffset.y = innerBounds.position.y - player.getPosition().y - battlebox.getOutlineThickness() + 1;
    }
    if ((playerPos.x + playerSize.x) > (innerBounds.position.x + innerBounds.size.x)) {
        moveOffset.x = (innerBounds.position.x + innerBounds.size.x) - (player.getPosition().x + playerSize.x) +
                       battlebox.getOutlineThickness() - 1;;
    }
    if ((playerPos.y + playerSize.y) > (innerBounds.position.y + innerBounds.size.y)) {
        moveOffset.y = (innerBounds.position.y + innerBounds.size.y) - (player.getPosition().y + playerSize.y) +
                       battlebox.getOutlineThickness() - 1;;
    }
}

void Game::updateBullets() {
    for (auto& bulletobj : bullet) {
        bulletobj.update();
        // std::cout << "bullet updated" << std::endl;
    }
    std::erase_if(bullet,
                  [this](const Bullet& b) {return b.isOffScreen(window); });
    std::erase_if(bullet,
                  [this](const Bullet& b) {
                      return std::nullopt!=player.getGlobalBounds().findIntersection(b.getGlobalBounds());
                  });
}

void Game::update() {
    sf::Vector2f moveOffset = calculateMoveOffset();
    enforceBattleBoxBounds(moveOffset);
    player.move(moveOffset);

    updateBullets();
}

void Game::render() {
    window.clear();
    battlebox.draw(window);
    // std::cout << "battlebox coordonates " << battlebox.getGlobalBounds().position.x << " " << battlebox.
    //         getGlobalBounds().position.y << std::endl;
    player.draw(window);
    // std::cout << "Player: " << player << std::endl;
    for (const auto& bulletobj : bullet) {
        bulletobj.draw(window);
        // std::cout <<"drawed ::::::"<< bulletobj<<std::endl;
    }
    window.display();
}

bool Game::isBulletsActive() const { return !bullet.empty();}

Game::Game() : window(sf::VideoMode({640, 480}), "Game", sf::Style::Titlebar | sf::Style::Close),
               battlebox({242, 150}, {155, 130})
{
    window.setFramerateLimit(30);
    battlebox.setBottomY(385.f);
    centerPlayer();

    // temporary
    for (int i=0;i<5;i++) {
        bullet.emplace_back(flybullet, sf::Vector2f{100.f, static_cast<float>(100 + (i + 1) * 50)},
                            sf::Vector2f{1.0f, 0.0f}); // fish
        std::cout<<"bullet "<<i+1<<" created"<<std::endl;
    }

}

std::ostream & operator<<(std::ostream &os, const Game &obj) {
    return os
           << "Game Status - Player: " << obj.player
           << " , Battle Box: " << obj.battlebox
           << " , Bullets Active: " << (obj.isBulletsActive() ? "Yes" : "No");
}

void Game::run() {
    std::cout <<"Running..." << std::endl;

    const SplashScreen splash;
    splash.show(window);
    const SplashScreen instructions{"./img/black.png"," --- Instruction --- \n\n"
                                                               "[Z or ENTER] - Confirm\n"
                                                               "[X or SHIFT] - Cancel\n"
                                                               "[F4] - Fullscreen\n"
                                                               "[ESC] - Quit\n"
                                                               "When HP is 0, you lose.\n\n"
                                                               "[Press Z or ENTER] to begin game",{170,100}, 30};
    instructions.show(window);

    playMusBattle1();
    while (window.isOpen()) {
        handleEvents();
        update();
        render();
    }
}