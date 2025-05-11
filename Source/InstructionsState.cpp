#include "../Headers/InstructionsState.h"
#include "../Headers/PlayState.h"

InstructionsState::InstructionsState(const std::string &text, const sf::Vector2f positionText,
                         const unsigned int fontSize) : instrText(*gameManager.fonts.get("./fonts/DeterminationSans.ttf"),text, fontSize) {
    instrText.setFillColor(sf::Color::White);
    instrText.setPosition(positionText);
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
    if(shouldTransition) {
        gameManager.playMusic("./mus/mus_ohyes.ogg");
        gameManager.pushState(std::make_unique<PlayState>());
    }
}

void InstructionsState::doRender(sf::RenderWindow& window) const {
    window.clear();
    window.draw(instrText);
}

bool InstructionsState::shouldChangeState() const {
    return shouldTransition;
}