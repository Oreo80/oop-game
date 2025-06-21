#pragma once
#include "GameState.h"
#include "Player.h"
#include <set>
#include <SFML/Audio.hpp>
#include "Hp.h"
#include "BattleUISystem.h"
#include "BulletManager.h"
#include "DeathAnimationManager.h"
#include "PlayerTurnSystem.h"
#include "EnemyTurnSystem.h"
class PlayState : public GameState {
private:
    bool shouldTransition = false;
    Player player;
    std::set<sf::Keyboard::Scancode> keysPressed;
    BattleUISystem ui;
    BulletManager bulletManager;
    DeathAnimationManager deathManager;
    PlayerTurnSystem playerTurnSystem;
    EnemyTurnSystem enemyTurnSystem;
    enum class TurnState {
        PlayerTurn,
        EnemyTurn
    };
    TurnState currentTurn = TurnState::PlayerTurn;
    void doProcessEvent(const std::optional<sf::Event> &event) override;
    void doUpdate() override;
    void doRender(sf::RenderWindow& window) override;
protected:
    void print(std::ostream &os) const override;
public:
    PlayState();
    ~PlayState() override = default;
    // std::unique_ptr<GameState> clone() const override;
    bool shouldChangeState() const override;
    std::unique_ptr<GameState> nextState() override;
};