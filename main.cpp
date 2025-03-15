#include <iostream>
#include <chrono>
//#include <thread>
//#include <cmath>
#include <ostream>
#include <set>
#include <SFML/Graphics.hpp>

class Player {
private:
    sf::Texture texture;
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

    friend std::ostream & operator<<(std::ostream &os, const Player &obj) {
        return os
               << "Position: (" << obj.position.x << ", " << obj.position.y << ")";
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
    explicit BulletType(const std::vector<std::string>& paths={"../img/spr_flybullet_0.png", "../img/spr_flybullet_1.png"},
        const float spd=5.0f, const sf::Vector2f dir={ 1.0f, 0.0f })
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

    friend std::ostream & operator<<(std::ostream &os, const BulletType &obj) {
        return os
               << "Speed: " << obj.speed
               << ", Direction: (" << obj.direction.x << ", " << obj.direction.y
               << ", Textures Loaded: " << obj.textures.size();
    }

    [[nodiscard]] const std::vector<sf::Texture>& getTextures() const { return textures; }
    [[nodiscard]] float getSpeed() const { return speed; }
    [[nodiscard]] sf::Vector2f getDirection() const { return direction; }

    void set_direction(const sf::Vector2f &direction) {
        this->direction = direction;
    }

    BulletType(const BulletType &other)= default;

    BulletType(BulletType &&other) noexcept
        : textures(std::move(other.textures)),
          speed(other.speed),
          direction(other.direction) {
    }

    BulletType & operator=(const BulletType &other) {
        if (this == &other)
            return *this;
        textures = other.textures;
        speed = other.speed;
        direction = other.direction;
        return *this;
    }

    BulletType & operator=(BulletType &&other) noexcept {
        if (this == &other)
            return *this;
        textures = std::move(other.textures);
        speed = other.speed;
        direction = other.direction;
        return *this;
    }
};

class Bullet {
private:
    size_t currentFrame = 0;
    sf::Clock animationClock;
    BulletType bulletType;
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Sprite sprite;

    static const BulletType& getDefaultBulletType() {
        static const BulletType defaultbullet;
        return defaultbullet;
    }
public:
    explicit Bullet(const BulletType& type=getDefaultBulletType(), const sf::Vector2f& startPos={100.f,100.f})
    : bulletType(type),position(startPos), velocity(type.getDirection() * type.getSpeed()), sprite(type.getTextures()[0])
    {
        sprite.setPosition(position);
        std::cout<<"constructor initializare\n";
    }

    Bullet(const Bullet &other) = default;

    Bullet(Bullet &&other) noexcept
        : currentFrame(other.currentFrame),
          animationClock(other.animationClock),
          bulletType(std::move(other.bulletType)),
          position(other.position),
          velocity(other.velocity),
          sprite(std::move(other.sprite)) {
    }

    Bullet & operator=(const Bullet &other) {
        if (this == &other)
            return *this;
        currentFrame = other.currentFrame;
        animationClock = other.animationClock;
        bulletType = other.bulletType;
        position = other.position;
        velocity = other.velocity;
        sprite = other.sprite;
        return *this;
    }

    Bullet & operator=(Bullet &&other) noexcept {
        if (this == &other)
            return *this;
        currentFrame = other.currentFrame;
        animationClock = other.animationClock;
        bulletType = std::move(other.bulletType);
        position = other.position;
        velocity = other.velocity;
        sprite = std::move(other.sprite);
        return *this;
    }

    ~Bullet() {std::cout<<"bullet destructor\n";}

    friend std::ostream & operator<<(std::ostream &os, const Bullet &obj) {
        return os
            << "Position: (" << obj.position.x << ", " << obj.position.y << ") "
            << ", Velocity: ( "<< obj.velocity.x << ", " << obj.velocity.y
            << " , Current Frame: " << obj.currentFrame
            << " , BulletType: " << obj.bulletType;

    }

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
        // std::cout << "Player: " << player << std::endl;
        if (bullet) {
            bullet->draw(window);
            // std::cout << "Bullet: " << *bullet << std::endl;
        }
        window.display();
    }
public:
    Game() : window(sf::VideoMode({640, 480}), "My Window", sf::Style::Titlebar | sf::Style::Close),
            flybullet({ "../img/spr_flybullet_0.png", "../img/spr_flybullet_1.png" }, 5.0f, { 1.0f, 0.0f }),
            bullet(std::make_unique<Bullet>(flybullet, sf::Vector2f{100.f, 100.f}))
    {
        window.setFramerateLimit(30);
    }

    bool isBulletActive() const { return bullet != nullptr; }

    friend std::ostream & operator<<(std::ostream &os, const Game &obj) {
        return os
               << "Game Status - Player: " << obj.player
               << " , Bullet Active: " << (obj.isBulletActive() ? "Yes" : "No");
    }

    void run() {
        std::cout <<"Running..." << std::endl;
        while (window.isOpen()) {
            handle_events();
            update();
            render();

        }
    }
};
int main() {
    // BulletType flybullet({ "../img/spr_flybullet_0.png", "../img/spr_flybullet_1.png" }, 5.0f, { 1.0f, 0.0f });
    // Bullet bullet(flybullet, sf::Vector2f{100.f, 100.f});
    // Bullet A,B,C,D,E,F,G,H;
    // A=B=C=D=E=F=G=H=bullet;
    // D=std::move(F);
    // Bullet EE(std::move(D));

    try {
        Game game;
        game.run();
        std::cout << "Game exit status: " << game << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }
    std::cout << "Programul a terminat execuția\n";
    return 0;
}
