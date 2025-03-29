#pragma once

#include "Bullet.h"
#include "Player.h"
#include "BattleBox.h"
#include "Button.h"
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
    Button fightButton{"./img/spr_fightbt_0.png",{32,432}},
            talkButton{"./img/spr_talkbt_0.png",{185,432}},
            itemButton{"./img/spr_itembt_0.png",{345,432}},
            spareButton{"./img/spr_sparebt_0.png",{500,432}};
    bool isFullscreen=false;
    void toggleFullscreen();
    void updateView();
    void playMusBattle1();
    void centerPlayer();
    void handleEvents();
    sf::Vector2f calculateMoveOffset() const;
    void enforceBattleBoxBounds(sf::Vector2f& moveOffset) const;
    void updateBullets();
    void update();
    void render();
    bool isBulletsActive() const;
public:
    Game();
    friend std::ostream & operator<<(std::ostream &os, const Game &obj);
    void run();
};
