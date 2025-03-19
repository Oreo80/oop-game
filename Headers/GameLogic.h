#pragma once

#include "Bullet.h"
#include "Player.h"
#include <set>

class Game {
private:
    sf::RenderWindow window;
    Player player;
    BulletType flybullet;
    std::vector<Bullet> bullet;
    std::set<sf::Keyboard::Scancode> keysPressed;
    void handle_events();
    void update();
    void render();
    bool isBulletsActive() const;
public:
    Game();
    friend std::ostream & operator<<(std::ostream &os, const Game &obj);
    void run();
};