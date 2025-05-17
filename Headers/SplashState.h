#pragma once
#include "GameState.h"
#include "SpriteEntity.h"
#include "BitmapFont.h"

class SplashState : public GameState {
private:
    SpriteEntity background;
    BitmapFont font;
    bool shouldTransition = false;

    void doProcessEvent(const std::optional<sf::Event> &event) override;
    void doUpdate() override;
    void doRender(sf::RenderWindow& window) const override;
protected:
    void print(std::ostream& os) const override;
public:
    explicit SplashState(const std::string &filePath="./img/splash.png",const std::string &text="[PRESS Z OR ENTER]",
                            sf::Vector2f positionText=sf::Vector2f(50, 400), float scale = 0.8f);
    ~SplashState() override = default;

    // std::unique_ptr<GameState> clone() const override;
    bool shouldChangeState() const override;
    std::unique_ptr<GameState> nextState() override;

    friend std::ostream& operator<<(std::ostream &os, const SplashState &state);
};