#pragma once

#include "Bullet.h"
#include "Player.h"
#include "BattleBox.h"
#include <set>
#include <SFML/Audio.hpp>

class Game {
private:
    sf::RenderWindow window;
    Player player;
    BulletType flybullet;
    std::vector<Bullet> bullet;
    BattleBox battlebox;
    std::set<sf::Keyboard::Scancode> keysPressed;
    sf::Music mus_battle1;
    void playMusBattle1();
    void handleEvents();
    void update();
    void render();
    bool isBulletsActive() const;
public:
    Game();
    friend std::ostream & operator<<(std::ostream &os, const Game &obj);
    void run();
};