#include <iostream>
#include <chrono>
//#include <thread>

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
};
class Game {
private:
    sf::RenderWindow window;
    Player player;
    void handle_events() {
            bool shouldExit = false;
            while(const std::optional event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) {
                    window.close();
                }
                if (event->is<sf::Event::KeyPressed>()) {
                    const auto* keyPressed = event->getIf<sf::Event::KeyPressed>();
                    if(keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                        shouldExit = true;
                    }
                    if(keyPressed->scancode == sf::Keyboard::Scancode::Up) {player.setPosition(player.getPosition() + sf::Vector2f(0, -1));}
                    if(keyPressed->scancode == sf::Keyboard::Scancode::Down) {player.setPosition(player.getPosition() + sf::Vector2f(0, 1));}
                    if(keyPressed->scancode == sf::Keyboard::Scancode::Left) {player.setPosition(player.getPosition() + sf::Vector2f(-1, 0));}
                    if(keyPressed->scancode == sf::Keyboard::Scancode::Right) {player.setPosition(player.getPosition() + sf::Vector2f(1, 0));}
                }
            }
            if(shouldExit) {
                window.close();
            }
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
