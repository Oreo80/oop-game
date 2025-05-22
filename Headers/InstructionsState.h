#pragma once
#include "GameState.h"

class InstructionsState : public GameState {
private:
    BitmapFont font;
    bool shouldTransition = false;

    void doProcessEvent(const std::optional<sf::Event> &event) override;
    void doUpdate() override;
    void doRender(sf::RenderWindow& window) override;
protected:
    void print(std::ostream &os) const override;
public:
    explicit InstructionsState(const std::string &text=" --- Instructions --- \n\n"
                                                               "[Z or ENTER] - Confirm\n"
                                                               "[X or SHIFT] - Cancel\n"
                                                               "[F4] - Fullscreen\n"
                                                               "[ESC] - Quit\n"
                                                               "When HP is 0, you lose.\n\n"
                                                               "[Press Z or ENTER] to begin game",
                            sf::Vector2f positionText=sf::Vector2f(170, 100), float scale = 1.f);
    ~InstructionsState() override = default;

    // std::unique_ptr<GameState> clone() const override;
    bool shouldChangeState() const override;
    std::unique_ptr<GameState> nextState() override;
};