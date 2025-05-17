#include "../Headers/GameState.h"

void GameState::handleGlobalEvent(const std::optional<sf::Event> &event) const {
    if (event->is<sf::Event::KeyPressed>()) {
        const auto* keyPressed = event->getIf<sf::Event::KeyPressed>();

        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
            windowPtr->close();
        }
        if (keyPressed->scancode == sf::Keyboard::Scancode::F4) {
            gameManager.toggleFullscreen();
        }
    }
}

GameState::GameState(): gameManager(GameManager::getInstance()), windowPtr(gameManager.getWindow()) {}

std::ostream & operator<<(std::ostream &os, const GameState &state) {
    state.print(os);
    return os;
}
