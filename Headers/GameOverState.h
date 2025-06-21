#pragma once
#include "GameState.h"
#include "SpriteEntity.h"
#include "BattleText.h"
#include "PlayState.h"
#include <random>
class GameOverState : public GameState {
private:
    SpriteEntity background;
    BattleText text;
    bool shouldTransition = false;
public:
    GameOverState();
    void doRender(sf::RenderWindow &window) override;
    void doUpdate() override;
    void doProcessEvent(const std::optional<sf::Event> &event) override;
    bool shouldChangeState() const override;
    std::unique_ptr<GameState> nextState() override;
};
