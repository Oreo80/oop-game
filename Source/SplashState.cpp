#include "../Headers/SplashState.h"
#include "../Headers/GameManager.h"
#include "../Headers/InstructionsState.h"
SplashState::SplashState(const std::string &filePath ,const std::string &text, const sf::Vector2f positionText,
                           const float scale) : background(filePath,{0,0}),
font("./fonts/fnt_main.png","./fonts/glyphs_fnt_main.csv",text,positionText,sf::Color::White, scale){
    gameManager.playMusic("./mus/mus_menu0.ogg");
}

// std::unique_ptr<GameState> SplashState::clone() const {
//     return std::make_unique<SplashState>(*this);
// }

void SplashState::doProcessEvent(const std::optional<sf::Event> &event) {
    if (event->is<sf::Event::KeyPressed>()) {
        if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>();
        keyPressed->scancode == sf::Keyboard::Scancode::Enter || keyPressed->scancode == sf::Keyboard::Scancode::Z) {
            shouldTransition = true;
           }
    }
}
void SplashState::doUpdate() {
    if(shouldTransition) {
        gameManager.pushState(std::make_unique<InstructionsState>());
    }
}

void SplashState::doRender(sf::RenderWindow& window) const {
    background.draw(window);
    const_cast<BitmapFont&>(font).drawText(window);
}

bool SplashState::shouldChangeState() const {
    return shouldTransition;
}