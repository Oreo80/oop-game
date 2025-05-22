#pragma once
#include "GameState.h"
#include "Bullet.h"
#include "Player.h"
#include "BattleBox.h"
#include "Button.h"
#include <set>
#include <SFML/Audio.hpp>
#include "BattleText.h"
#include "Hp.h"
#include "Froggit.h"
#include "ShardEntity.h"
class PlayState : public GameState {
private:
    SpriteEntity background;
    bool shouldTransition = false;
    Player player;
    std::vector<std::unique_ptr<DrawableEntity>> bullets;
    BattleBox battleBox;
    std::set<sf::Keyboard::Scancode> keysPressed;
    BattleText battleText;
    Hp hp;
    Froggit froggit;
    Button fightButton{"./img/spr_fightbt_0.png","./img/spr_fightbt_1.png",{32,432}},
            talkButton{"./img/spr_talkbt_0.png","./img/spr_talkbt_1.png",{185,432}},
            itemButton{"./img/spr_itembt_0.png","./img/spr_itembt_1.png",{345,432}},
            spareButton{"./img/spr_sparebt_0.png","./img/spr_sparebt_1.png",{500,432}};
    enum class TurnState {
        PlayerTurn,
        EnemyTurn
    };
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
    std::vector<Button*> getButtons() const;
    sf::Vector2f calculateMoveOffset() const;
    void enforceBattleBoxBounds(sf::Vector2f& moveOffset) const;

    void startDeath();

    void processDamage();
    void cleanupBullets();
    void enterPlayerTurn();
    void updatePlayerTurn();
    void enterEnemyTurn();
    void updateEnemyTurn();
    void updateButtonTextures() const;
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

    void runSelfTest() const;

    // std::unique_ptr<GameState> nextState() override;
};