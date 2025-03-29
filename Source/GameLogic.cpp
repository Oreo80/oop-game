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

    const float windowRatio = static_cast<float>(winSize.x) / static_cast<float>(winSize.y);
    const float targetRatio = targetSize.x / targetSize.y;

    sf::View view(sf::FloatRect({0, 0}, {targetSize.x, targetSize.y}));

    if (windowRatio > targetRatio) {
        const float newWidth = targetRatio * static_cast<float>(winSize.y);
        view.setViewport(sf::FloatRect({(1.f - newWidth / static_cast<float>(winSize.x)) / 2.f, 0.f},
                                       {newWidth / static_cast<float>(winSize.x), 1.f}));
    } else {
        const float newHeight = static_cast<float>(winSize.x) / targetRatio;
        view.setViewport(sf::FloatRect({0.f, (1.f - newHeight / static_cast<float>(winSize.y)) / 2.f},
                                       {1.f, newHeight / static_cast<float>(winSize.y)}));
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
    if (const sf::FloatRect newPlayerBounds(playerPos, playerSize);
        std::nullopt == innerBounds.findIntersection(newPlayerBounds)) {
        return;
    }

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
    for (auto& bulletobj : bullets) {
        bulletobj.update();
        // std::cout << "bullet updated" << std::endl;
    }
    std::erase_if(bullets,
                  [this](const Bullet& bulletobj) {return bulletobj.isOffScreen(window); });
    std::erase_if(bullets,
                  [this](const Bullet& bulletobj) {
                      return std::nullopt!=player.getGlobalBounds().findIntersection(bulletobj.getGlobalBounds());
                  });
}

void Game::update() {
    switch (currentTurn) {
        case TurnState::PlayerTurn:
            updateActionSelection();
            break;
        case TurnState::EnemyTurn:
            updateEnemyTurn();
            break;
    }
}

void Game::updateActionSelection() {
    if (keysPressed.contains(sf::Keyboard::Scancode::Left)) {
        currentActionIndex = (currentActionIndex - 1 + static_cast<int>(actionButtons.size())) % static_cast<int>(
                                 actionButtons.size());
        player.setPosition(actionButtons[currentActionIndex]->getPositionForPlayer());
        keysPressed.erase(sf::Keyboard::Scancode::Left);
        updateButtonTextures();
    }
    if (keysPressed.contains(sf::Keyboard::Scancode::Right)) {
        currentActionIndex = (currentActionIndex + 1 + static_cast<int>(actionButtons.size())) % static_cast<int>(
                                 actionButtons.size());
        player.setPosition(actionButtons[currentActionIndex]->getPositionForPlayer());
        keysPressed.erase(sf::Keyboard::Scancode::Right);
        updateButtonTextures();
    }

    if (keysPressed.contains(sf::Keyboard::Scancode::Enter) || keysPressed.contains(sf::Keyboard::Scancode::Z)) {
        processSelectedAction(currentActionIndex);
        actionButtons[currentActionIndex]->setTexture(false);
        enterEnemyTurn();
    }
}
void Game::enterEnemyTurn() {
    currentTurn = TurnState::EnemyTurn;
    keysPressed.clear();
    centerPlayer();
    enemyTurnClock.restart();

    for (int i=0;i<5;i++) {
        bullets.emplace_back(flybullet, sf::Vector2f{100.f, static_cast<float>(225 + (i + 1) * 30)},
                            sf::Vector2f{1.0f, 0.0f}); // fish
    }
}

void Game::updateEnemyTurn() {
    sf::Vector2f moveOffset = calculateMoveOffset();
    enforceBattleBoxBounds(moveOffset);
    player.move(moveOffset);

    updateBullets();

    if (!isBulletsActive() || enemyTurnClock.getElapsedTime().asSeconds() >= enemyTurnDuration) {
        bullets.clear();
        currentTurn = TurnState::PlayerTurn;
        enterActionSelection();
    }
}

void Game::enterActionSelection() {
    currentActionIndex = 0;
    player.setPosition(fightButton.getPositionForPlayer());
    actionButtons[0]->setTexture(true);
    keysPressed.clear();
}
void Game::updateButtonTextures() const {
    for (size_t i = 0; i < actionButtons.size(); i++) {
        actionButtons[i]->setTexture(i == static_cast<size_t>(currentActionIndex));
    }
}

void Game::processSelectedAction(const int actionIndex) {
    switch (actionIndex) {
        case 0: // Fight
            std::cout << "Fight selected! Player attacks!\n";
        break;
        case 1: // Talk
            std::cout << "Talk selected! Player tries to communicate.\n";
        break;
        case 2: // Item
            std::cout << "Item selected! Open inventory.\n";
        break;
        case 3: // Spare
            std::cout << "Spare selected! Attempting mercy...\n";
        break;
        default:
            std::cerr << "Invalid action index!\n";
    };
}


void Game::render() {
    window.clear();
    battlebox.draw(window);
    fightButton.draw(window);
    talkButton.draw(window);
    itemButton.draw(window);
    spareButton.draw(window);
    player.draw(window);
    for (const auto& bulletobj : bullets) {
        bulletobj.draw(window);
    }
    window.display();
}

bool Game::isBulletsActive() const { return !bullets.empty();}

Game::Game() : window(sf::VideoMode({640, 480}), "Game", sf::Style::Titlebar | sf::Style::Close),
               battlebox({242, 150}, {155, 130})
{
    window.setFramerateLimit(30);
    battlebox.setBottomY(385.f);
    centerPlayer();

    // temporary
    // for (int i=0;i<5;i++) {
    //     bullets.emplace_back(flybullet, sf::Vector2f{100.f, static_cast<float>(225 + (i + 1) * 30)},
    //                         sf::Vector2f{1.0f, 0.0f}); // fish
    // }
    actionButtons = { &fightButton, &talkButton, &itemButton, &spareButton };

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
    enterActionSelection();
    while (window.isOpen()) {
        handleEvents();
        update();
        render();
    }
}