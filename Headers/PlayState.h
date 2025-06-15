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
class PlayState : public GameState {
private:
    bool shouldTransition = false;
    Player player;
    std::set<sf::Keyboard::Scancode> keysPressed;
    BattleUISystem ui;
    BulletManager bulletManager;
    DeathAnimationManager deathManager;
    PlayerTurnSystem playerTurnSystem;
    struct Item {
        std::string realName;
        std::string shortName;
        int healAmount;
    };
    enum class TurnState {
        PlayerTurn,
        EnemyTurn
    };
    TurnState currentTurn = TurnState::PlayerTurn;
    sf::Clock enemyTurnClock;
    static const float enemyTurnDuration;
    bool waitingForTextDelay=false;
    void doProcessEvent(const std::optional<sf::Event> &event) override;
    void doUpdate() override;
    void doRender(sf::RenderWindow& window) override;
    sf::Vector2f calculateMoveOffset() const;
    void enforceBattleBoxBounds(sf::Vector2f& moveOffset);
    void processDamage();
    void enterEnemyTurn();
    void updateEnemyTurn();
    bool areBulletsActive() const;
protected:
    void print(std::ostream &os) const override;
public:
    PlayState();
    ~PlayState() override = default;
    PlayState(const PlayState& other);
    PlayState& operator=(PlayState other);
    friend void swap(PlayState& first, PlayState& second) noexcept;
    // std::unique_ptr<GameState> clone() const override;
    bool shouldChangeState() const override;
    // std::unique_ptr<GameState> nextState() override;
};