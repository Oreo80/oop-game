#pragma once
#include "GameState.h"
#include "Bullet.h"
#include "Player.h"
#include <set>
#include <SFML/Audio.hpp>
#include "BattleText.h"
#include "Hp.h"
#include "Froggit.h"
#include "ShardEntity.h"
#include "BattleUISystem.h"
#include "BulletManager.h"
class PlayState : public GameState {
private:
    BattleUISystem ui;
    BulletManager bulletManager;
    struct Item {
        std::string realName;
        std::string shortName;
        int healAmount;
    };
    struct MenuState {
        std::vector<std::string> options;
        int currentIndex = 0;
        bool inMessagePhase = false;
        std::function<void(int)> onSelect;
    };
    MenuState actMenuState;
    MenuState itemMenuState;
    bool shouldTransition = false;
    Player player;
    // std::vector<std::unique_ptr<DrawableEntity>> bullets;
    std::set<sf::Keyboard::Scancode> keysPressed;
    bool actionConfirmed = false;
    bool mercyConditionsMet = false;
    std::vector<Item> inventory = {
        {"Monster Candy", "MnstrCndy", 10},
        {"Spider Donut", "SpidrDont", 12},
        {"Butterscotch Pie", "ButtsPie", 20},
        { "Spider Cider", "SpidrCidr", 18}
    };
    bool victoryAchieved = false;
    int victoryFrame = 0;
    std::vector<std::string> actOptions;
    enum class TurnState {
        PlayerTurn,
        EnemyTurn,
        SubMenu
    };
    enum class SubMenuState {
        None,
        Fight,
        Talk,
        Item,
        Spare
    };
    SubMenuState currentSubMenu = SubMenuState::None;
    size_t savedActionIndex = 0;
    std::vector<DrawableEntity*> entities;
    TurnState currentTurn = TurnState::PlayerTurn;
    sf::Clock enemyTurnClock;
    static const float enemyTurnDuration;
    int currentActionIndex = 0;
    bool waitingForTextDelay=false;
    enum class DeathStage {
        None,
        ShowPlayer,
        ShowBrokenHeart,
        ShowShards,
        FadeOut
    };
    DeathStage deathStage = DeathStage::None;
    int deathFrame = -1;
    std::vector<std::unique_ptr<ShardEntity>> shards;
    void doProcessEvent(const std::optional<sf::Event> &event) override;
    static void spawnShards(std::vector<std::unique_ptr<ShardEntity>>& shards, const sf::Vector2f& playerPos, int shardCount = 6);
    static sf::Vector2f generateShardVelocity(int index, int totalShards);
    void updateDeath();
    void renderDeath(sf::RenderWindow &window);
    void doUpdate() override;
    void doRender(sf::RenderWindow& window) override;
    void initEntities();
    sf::Vector2f calculateMoveOffset() const;
    void enforceBattleBoxBounds(sf::Vector2f& moveOffset);
    void startDeath();
    void processDamage();
    // void cleanupBullets();
    void enterPlayerTurn();
    void updatePlayerTurn();
    void enterEnemyTurn();
    void updateEnemyTurn();
    void updateMenu(MenuState &menuState);
    void enterSubMenu();
    void exitSubMenu();
    void updateMercyMenu();
    void useItem(int itemIndex);
    void processActSelection(int actIndex);
    static void updateFightMenu();
    void updateSubMenu();
    void enterFightSubMenu();
    void enterTalkSubMenu();
    void enterItemSubMenu();
    void enterMercySubMenu();
    void processSelectedAction(int actionIndex);
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