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
class BulletType {
private:
    std::vector<sf::Texture> textures;
    float speed;
    sf::Vector2f direction;

public:
    BulletType(const std::vector<std::string>& paths, const float spd, const sf::Vector2f dir)
        : speed(spd), direction(dir)
    {
        for (const auto& path : paths) {
            sf::Texture tex;
            if (!tex.loadFromFile(path)) {
                throw std::runtime_error("Failed to load texture: " + path);
            }
            textures.push_back(std::move(tex));
        }

        if (textures.empty()) {
            throw std::runtime_error("BulletType requires at least one texture");
        }
    }

    [[nodiscard]] const std::vector<sf::Texture>& getTextures() const { return textures; }
    [[nodiscard]] float getSpeed() const { return speed; }
    [[nodiscard]] sf::Vector2f getDirection() const { return direction; }
};

class Bullet {
private:
    size_t currentFrame = 0;
    sf::Clock animationClock;
    const BulletType& bulletType;
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Sprite sprite;

public:
    explicit Bullet(const BulletType& type, const sf::Vector2f& startPos)
    : bulletType(type),
      position(startPos),
      velocity(type.getDirection() * type.getSpeed()),
      sprite(type.getTextures()[0])
    {
        sprite.setPosition(position);
    }

    ~Bullet() {std::cout<<"bullet destructor\n";}

    void update() {
        position += velocity;
        sprite.setPosition(position);

        if (animationClock.getElapsedTime().asMilliseconds() > 100) {
            currentFrame = (currentFrame + 1) % bulletType.getTextures().size();
            sprite.setTexture(bulletType.getTextures()[currentFrame]);
            animationClock.restart();
        }
    }

    void draw(sf::RenderWindow& window) const {
        window.draw(sprite);
    }

    bool isOffScreen(const sf::RenderWindow& window) const {
        const float windowWidth = static_cast<float>(window.getSize().x);
        const float windowHeight = static_cast<float>(window.getSize().y);

        return position.x < 0 || position.x > windowWidth ||
               position.y < 0 || position.y > windowHeight;
    }

};


class Game {
private:
    sf::RenderWindow window;
    Player player;
    BulletType flybullet;
    std::unique_ptr<Bullet> bullet;
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

        player.move(moveOffset);
        if (bullet) {
            bullet->update();
            if (bullet->isOffScreen(window)) {
                bullet.reset();
            }
        }
    }
    void render() {
        window.clear();
        player.draw(window);
        if (bullet) bullet->draw(window);
        window.display();
    }
public:
    Game() : window(sf::VideoMode({640, 480}), "My Window", sf::Style::Titlebar | sf::Style::Close),
            flybullet({ "../img/spr_flybullet_0.png", "../img/spr_flybullet_1.png" }, 5.0f, { 1.0f, 0.0f }),
            bullet(std::make_unique<Bullet>(flybullet, sf::Vector2f{100.f, 100.f}))
    {
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
