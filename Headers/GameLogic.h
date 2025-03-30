#pragma once

#include "Bullet.h"
#include "Player.h"
#include "BattleBox.h"
#include "Button.h"
#include "Background.h"
#include <set>
#include <SFML/Audio.hpp>

#include "BattleText.h"

class Game {
private:
    sf::RenderWindow window;
    Player player;
    BulletType flybullet;
    std::vector<Bullet> bullets;
    BattleBox battleBox;
    Background background;
    std::set<sf::Keyboard::Scancode> keysPressed;
    sf::Music mus_battle1;
    BattleText battleText;

    Button fightButton{"./img/spr_fightbt_0.png","./img/spr_fightbt_1.png",{32,432}},
            talkButton{"./img/spr_talkbt_0.png","./img/spr_talkbt_1.png",{185,432}},
            itemButton{"./img/spr_itembt_0.png","./img/spr_itembt_1.png",{345,432}},
            spareButton{"./img/spr_sparebt_0.png","./img/spr_sparebt_1.png",{500,432}};
    enum class TurnState {
        PlayerTurn,
        EnemyTurn
    };
    TurnState currentTurn = TurnState::PlayerTurn;
    sf::Clock enemyTurnClock;
    const float enemyTurnDuration = 10.f;
    int currentActionIndex = 0;
    std::vector<Button*> actionButtons;
    bool waitingForTextDelay=false;
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
    void updateActionSelection();

    void enterEnemyTurn();

    void updateEnemyTurn();
    void enterActionSelection();

    void updateButtonTextures() const;

    void processSelectedAction(int actionIndex);

    void render();
    bool isBulletsActive() const;
public:
    Game();
    friend std::ostream & operator<<(std::ostream &os, const Game &obj);
    void run();
};
