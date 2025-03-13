#include <iostream>
#include <chrono>
//#include <thread>
//#include <cmath>
#include <set>
#include <SFML/Graphics.hpp>

class Player {
private:
    sf::Texture texture{};
    sf::Sprite sprite;
    sf::Vector2f position;
public:
    explicit Player(const std::string& texturePath="../img/spr_heart_0.png", const sf::Vector2f& pos={312, 232})
        : texture([&]() {
            sf::Texture tex;
            if (!tex.loadFromFile(texturePath)) {
                std::cerr << "Failed to load sprite texture!\n";
                throw std::runtime_error("Failed to load sprite texture");
            }
            return tex;
        }()),
          sprite(texture)
    {
        sprite.setPosition(pos);
        position = pos;
    }
    void draw(sf::RenderWindow& window) const {
        window.draw(sprite);
    }
    void setPosition(const sf::Vector2f& position_) {
        sprite.setPosition(position_);
    }

    sf::Vector2f getPosition() const {
        return sprite.getPosition();
    }
    void move(const sf::Vector2f& offset) {
        position += offset;
        sprite.setPosition(position);
    }
};
class Game {
private:
    sf::RenderWindow window;
    Player player;
    std::set<sf::Keyboard::Scancode> keysPressed;
    void handle_events() {
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

    void update() {
        constexpr float speed = 4.0f;
        sf::Vector2f moveOffset(0, 0);

        if (keysPressed.contains(sf::Keyboard::Scancode::Up)) moveOffset.y -= speed;
        if (keysPressed.contains(sf::Keyboard::Scancode::Down)) moveOffset.y += speed;
        if (keysPressed.contains(sf::Keyboard::Scancode::Left)) moveOffset.x -= speed;
        if (keysPressed.contains(sf::Keyboard::Scancode::Right)) moveOffset.x += speed;

        // Normalize diagonal movement to maintain consistent speed
        // if (moveOffset.x != 0 && moveOffset.y != 0) {
        //     moveOffset /= std::sqrt(2.f);
        // }

        player.move(moveOffset);
    }
    void render() {
        window.clear();
        player.draw(window);
        window.display();
    }
public:
    Game() : window(sf::VideoMode({640, 480}), "My Window", sf::Style::Titlebar | sf::Style::Close), player() {
        window.setFramerateLimit(30);
    }
    void run() {
        while (window.isOpen()) {
            handle_events();
            update();
            render();
        }
    }
};
int main() {
    try {
        Game game;
        game.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }
    std::cout << "Programul a terminat execuția\n";
    return 0;
}
