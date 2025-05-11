// #include "../Headers/SplashScreen.h"
//
// #include <iostream>
//
// sf::Font SplashScreen::loadFont() {
//     sf::Font font_;
//     if (!font_.openFromFile("./fonts/DeterminationSans.ttf")) {
//         std::cerr << "Failed to load font file!\n";
//         throw std::runtime_error("Failed to load font file");
//     }
//     return font_;
// }
//
// SplashScreen::SplashScreen(const std::string &filePath, const std::string &text, sf::Vector2f positionText,
//                            const unsigned int fontSize) : background(filePath,{0,0}), font(loadFont()), splashText(font,text, fontSize) {
//     splashText.setFillColor(sf::Color::White);
//     splashText.setPosition(positionText);
// }
//
// void SplashScreen::show(sf::RenderWindow& window) const {
//     while(window.isOpen()) {
//
//         while(const std::optional event = window.pollEvent()) {
//             if (event->is<sf::Event::Closed>()) {
//                 window.close();
//                 return;
//             }
//
//             else if (event->is<sf::Event::KeyPressed>()) {
//                 if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>();
//                     keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
//                     window.close();
//                 }
//                 else if (keyPressed->scancode == sf::Keyboard::Scancode::Enter || keyPressed->scancode ==
//                          sf::Keyboard::Scancode::Z) {
//                     return;
//                 }
//             }
//         }
//         window.clear();
//         background.draw(window);
//         window.draw(splashText);
//         window.display();
//     }
// }
// std::ostream & operator<<(std::ostream &os, const SplashScreen &obj) {
//
//     return os
//                 << "Splash screen string loaded: "<<std::string(obj.splashText.getString())
//                 << ", Splash text position: ("<<obj.splashText.getPosition().x<<", "<<obj.splashText.getPosition().y<<") ";
// }
