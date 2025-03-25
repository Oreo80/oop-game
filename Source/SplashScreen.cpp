#include "../Headers/SplashScreen.h"

void SplashScreen::show(sf::RenderWindow& window) const {
    while(window.isOpen()) {

        while(const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                return;
            }

            else if (event->is<sf::Event::KeyPressed>()) {
                if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>();
                    keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                    window.close();
                }
                else if (keyPressed->scancode == sf::Keyboard::Scancode::Enter || keyPressed->scancode ==
                         sf::Keyboard::Scancode::Z) {
                    return;
                }
            }
        }
        window.clear();
        window.draw(splashSprite);
        window.draw(pressKeyText);
        window.display();
    }
}
std::ostream & operator<<(std::ostream &os, const SplashScreen &obj) {

    return os
                << "Splash image position: ("<< obj.splashSprite.getPosition().x<<", "<<obj.splashSprite.getPosition().y<<") "
                << ", Splash screen string loaded: "<<std::string(obj.pressKeyText.getString())
                << ", Splash text position: ("<<obj.pressKeyText.getPosition().x<<", "<<obj.pressKeyText.getPosition().y<<") ";
}
