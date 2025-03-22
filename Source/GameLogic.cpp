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

    if (keysPressed.contains(sf::Keyboard::Scancode::Up) || keysPressed.contains(sf::Keyboard::Scancode::W)) moveOffset.y -= speed;
    if (keysPressed.contains(sf::Keyboard::Scancode::Down) || keysPressed.contains(sf::Keyboard::Scancode::S)) moveOffset.y += speed;
    if (keysPressed.contains(sf::Keyboard::Scancode::Left) || keysPressed.contains(sf::Keyboard::Scancode::A)) moveOffset.x -= speed;
    if (keysPressed.contains(sf::Keyboard::Scancode::Right) || keysPressed.contains(sf::Keyboard::Scancode::D)) moveOffset.x += speed;


    // player.move(moveOffset);

    // if (player.getGlobalBounds().position.x==(battlebox.getGlobalBounds().position.x+2)) { //si pt y similar, uhhh gonna be weird for R and D ngl
    //     std::cout<<"THING!!!!!!!!!!!!!!!!!!!!"<<std::endl;
    // }
    // if (player.getGlobalBounds().position.x==(battlebox.getGlobalBounds().position.x+2)) {
    //     std::cout<<"THING!!!!!!!!!!!!!!!!!!!!"<<std::endl;
    // }
    const sf::Vector2f rectPos = battlebox.getPosition();
    const sf::Vector2f rectSize = battlebox.getSize();
    const float thickness =  battlebox.getOutlineThickness();

    const sf::FloatRect innerBounds({rectPos.x + thickness, rectPos.y + thickness},
                              {rectSize.x - 2 * thickness, rectSize.y - 2 * thickness});

    const sf::Vector2f playerPos = player.getPosition() + moveOffset;
    const sf::Vector2f playerSize = {player.getGlobalBounds().size.x, player.getGlobalBounds().size.y};

    // bool touchingBorder =
    //     playerPos.x < innerBounds.position.x ||                               // left
    //     playerPos.y < innerBounds.position.y ||                                // top
    //     (playerPos.x + playerSize.x) > (innerBounds.position.x + innerBounds.size.x) ||  // right
    //     (playerPos.y + playerSize.y) > (innerBounds.position.y + innerBounds.size.y);    // bottom

    // if (touchingBorder) {
    //     std::cout << "touching\n";
    // }

    if (playerPos.x < innerBounds.position.x) {moveOffset.x = innerBounds.position.x - player.getPosition().x-battlebox.getOutlineThickness()+1;}
    if (playerPos.y < innerBounds.position.y) {moveOffset.y = innerBounds.position.y - player.getPosition().y-battlebox.getOutlineThickness()+1;}
    if ((playerPos.x + playerSize.x) > (innerBounds.position.x + innerBounds.size.x)) {
        moveOffset.x = (innerBounds.position.x + innerBounds.size.x) - (player.getPosition().x + playerSize.x)+battlebox.getOutlineThickness()-1;;
    }
    if ((playerPos.y + playerSize.y) > (innerBounds.position.y + innerBounds.size.y)) {
        moveOffset.y = (innerBounds.position.y + innerBounds.size.y) - (player.getPosition().y + playerSize.y)+battlebox.getOutlineThickness()-1;;
    }

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
    battlebox.draw(window);
    // std::cout<<"battlebox coordonates "<<battlebox.getGlobalBounds().position.x<<" "<<battlebox.getGlobalBounds().position.y<<std::endl;
    player.draw(window);
    // std::cout << "Player: " << player << std::endl;
    for (const auto& bulletobj : bullet) {
        bulletobj.draw(window);
        // std::cout <<"drawed ::::::"<< bulletobj<<std::endl;
    }
    window.display();
}

bool Game::isBulletsActive() const { return !bullet.empty();}

Game::Game() : window(sf::VideoMode({640, 480}), "Game", sf::Style::Titlebar | sf::Style::Close), battlebox({200, 150}, {155, 130})
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