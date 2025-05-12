#pragma once
#include "GameState.h"
#include "SpriteEntity.h"


class SplashState : public GameState {
private:
    SpriteEntity background;
    sf::Text splashText;
    bool shouldTransition = false;

    void doProcessEvent(const std::optional<sf::Event> &event) override;
    void doUpdate() override;
    void doRender(sf::RenderWindow& window) const override;

public:
    explicit SplashState(const std::string &filePath="./img/splash.png",const std::string &text="[PRESS Z OR ENTER]",
                            sf::Vector2f positionText=sf::Vector2f(50, 400), unsigned int fontSize=24);
    ~SplashState() override = default;

    [[maybe_unused]] std::unique_ptr<GameState> clone() const override;
    bool shouldChangeState() const override;

};