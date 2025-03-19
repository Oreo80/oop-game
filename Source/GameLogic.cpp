#include "../Headers/GameLogic.h"

void Game::play_mus_battle1() {
    if (!mus_battle1.openFromFile("./mus/mus_battle1.ogg")) {
        std::cerr << "Failed to load mus_battle1.ogg file" << std::endl;
        throw std::runtime_error("Failed to load mus_battle1.ogg file");
    }
    mus_battle1.setLooping(true);
    mus_battle1.play();
}
void Game::handle_events() {
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
        }
        if (event->is<sf::Event::KeyReleased>()) {
            const auto* keyReleased = event->getIf<sf::Event::KeyReleased>();
            keysPressed.erase(keyReleased->scancode);
        }
    }
}

void Game::update() {
    constexpr float speed = 4.0f;
    sf::Vector2f moveOffset(0, 0);

    if (keysPressed.contains(sf::Keyboard::Scancode::Up)) moveOffset.y -= speed;
    if (keysPressed.contains(sf::Keyboard::Scancode::Down)) moveOffset.y += speed;
    if (keysPressed.contains(sf::Keyboard::Scancode::Left)) moveOffset.x -= speed;
    if (keysPressed.contains(sf::Keyboard::Scancode::Right)) moveOffset.x += speed;

    player.move(moveOffset);
    for (auto& bulletobj : bullet) {
        bulletobj.update();
        // std::cout << "bullet updated" << std::endl;
    }
    std::erase_if(bullet,
                  [this](const Bullet& b) {return b.isOffScreen(window); });
    std::erase_if(bullet,
                  [this](const Bullet& b) {
                      return std::nullopt!=player.getGlobalBounds().findIntersection(b.getGlobalBounds());
                  }); //collision attempt laterz
}
void Game::render() {
    window.clear();
    player.draw(window);
    // std::cout << "Player: " << player << std::endl;
    for (const auto& bulletobj : bullet) {
        bulletobj.draw(window);
        // std::cout <<"drawed ::::::"<< bulletobj<<std::endl;
    }
    window.display();
}

bool Game::isBulletsActive() const { return !bullet.empty();}

Game::Game() : window(sf::VideoMode({640, 480}), "Game", sf::Style::Titlebar | sf::Style::Close)
{
    window.setFramerateLimit(30);
    for (int i=0;i<5;i++) {
        bullet.emplace_back(flybullet, sf::Vector2f{100.f, static_cast<float>(100+(i+1)*50)}, sf::Vector2f{1.0f, 0.0f}); //fish
        std::cout<<"bullet "<<i+1<<" created"<<std::endl;
    }
}

std::ostream & operator<<(std::ostream &os, const Game &obj) {
    return os
           << "Game Status - Player: " << obj.player
           << " , Bullets Active: " << (obj.isBulletsActive() ? "Yes" : "No");
}

void Game::run() {
    std::cout <<"Running..." << std::endl;
    play_mus_battle1();
    while (window.isOpen()) {
        handle_events();
        update();
        render();

    }
}