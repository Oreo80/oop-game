#pragma once
#include "GameState.h"
#include "Bullet.h"
#include "Player.h"
#include "BattleBox.h"
#include "Button.h"
#include <set>
#include <SFML/Audio.hpp>
#include "BattleText.h"
#include "HpBar.h"
#include "Froggit.h"

class PlayState : public GameState {
private:
    SpriteEntity background;
    bool shouldTransition = false;
    Player player;
    std::vector<std::unique_ptr<Bullet>> bullets;
    BattleBox battleBox;
    std::set<sf::Keyboard::Scancode> keysPressed;
    int maxHp;
    int currentHp;
    BitmapFont hpLabelText;
    BitmapFont hpValueText;
    BattleText battleText;
    HpBar hpBar;
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

    void doProcessEvent(const std::optional<sf::Event> &event) override;
    void doUpdate() override;
    void doRender(sf::RenderWindow& window) const override;
    void initEntities();
    std::vector<Button*> getButtons() const;
    sf::Vector2f calculateMoveOffset() const;
    void enforceBattleBoxBounds(sf::Vector2f& moveOffset) const;
    void updateHp(int newHp);
    void processDamage();
    void cleanupBullets();
    void enterPlayerTurn();
    void updatePlayerTurn();
    void enterEnemyTurn();
    void updateEnemyTurn();
    void updateButtonTextures() const;
    void processSelectedAction(int actionIndex);
    bool isBulletsActive() const;
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
    friend std::ostream& operator<<(std::ostream &os, const PlayState &state);
};