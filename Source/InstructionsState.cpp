#include "../Headers/InstructionsState.h"
#include "../Headers/PlayState.h"

InstructionsState::InstructionsState(const std::string &text, const sf::Vector2f positionText, const float scale) :
font("./fonts/fnt_main.png","./fonts/glyphs_fnt_main.csv",text, positionText,sf::Color::White, scale) {
    gameManager.playMusic("./mus/mus_menu0.ogg");
}

// std::unique_ptr<GameState> InstructionsState::clone() const {
//     return std::make_unique<InstructionsState>(*this);
// }

void InstructionsState::doProcessEvent(const std::optional<sf::Event> &event) {
    if (event->is<sf::Event::KeyPressed>()) {
        if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>();
        keyPressed->scancode == sf::Keyboard::Scancode::Enter || keyPressed->scancode == sf::Keyboard::Scancode::Z) {
            shouldTransition = true;
        }
    }
}
void InstructionsState::doUpdate() {

}

void InstructionsState::doRender(sf::RenderWindow& window) const {
    window.clear();
    const_cast<BitmapFont&>(font).drawText(window);
}

void InstructionsState::print(std::ostream &os) const {
    GameState::print(os);
    os<<"Instructions State";
}

bool InstructionsState::shouldChangeState() const {
    return shouldTransition;
}

std::unique_ptr<GameState> InstructionsState::nextState() {
    return std::make_unique<PlayState>();
}