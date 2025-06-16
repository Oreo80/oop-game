#pragma once
#include "Player.h"
#include "BattleUISystem.h"
#include "BulletManager.h"
#include "DeathAnimationManager.h"
#include <set>
#include <SFML/Graphics.hpp>

class EnemyTurnSystem {
private:
    Player& player;
    BattleUISystem& ui;
    BulletManager& bulletManager;
    DeathAnimationManager& deathManager;
    std::set<sf::Keyboard::Scancode>& keysPressed;
    GameManager& gameManager;

    sf::Clock enemyTurnClock;
    static constexpr float enemyTurnDuration = 6.0f;

    void processDamage() const;
    void enforceBattleBoxBounds(sf::Vector2f& moveOffset) const;
    sf::Vector2f calculateMoveOffset() const;
    bool areBulletsActive() const;

public:
    explicit EnemyTurnSystem(Player& player,
                   BattleUISystem& ui,
                   BulletManager& bulletManager,
                   DeathAnimationManager& deathManager,
                   std::set<sf::Keyboard::Scancode>& keysPressed,
                   GameManager& gameManager);

    void start();
    bool update() const;  // returns true when enemy turn is complete
};
