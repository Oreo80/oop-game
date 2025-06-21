#include "../Headers/GameOverState.h"

GameOverState::GameOverState(): background("./img/spr_gameoverbg.png"),
                                       text("./fonts/fnt_main.png","./fonts/glyphs_fnt_main.csv",
                                            {140,300}, 1.f,0.1f,0.5f) {
    text.setText("You cannot give up just yet... \nStay determined.");
    gameManager.fadeIn(1.f);
    gameManager.playMusic("./mus/mus_gameover.ogg");
}
void GameOverState::doRender(sf::RenderWindow &window) {
    background.draw(window);
    text.draw(window);
}

void GameOverState::doUpdate() {
    text.update();
}

void GameOverState::doProcessEvent(const std::optional<sf::Event> &event) {
    if (event->is<sf::Event::KeyPressed>()) {
        if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>();
            keyPressed->scancode == sf::Keyboard::Scancode::Enter || keyPressed->scancode == sf::Keyboard::Scancode::Z) {
            shouldTransition = true;
            gameManager.fadeOut(1.f);
            }
    }
}

bool GameOverState::shouldChangeState() const {
    return shouldTransition;
}

std::unique_ptr<GameState> GameOverState::nextState() {
    return std::make_unique<PlayState>();
}