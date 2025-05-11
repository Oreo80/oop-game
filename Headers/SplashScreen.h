#pragma once
// #include "SpriteEntity.h"
//
// class SplashScreen{
// private:
//     SpriteEntity background;
//     sf::Font font;
//     sf::Text splashText;
//
//     static sf::Font loadFont();
// public:
//     explicit SplashScreen(const std::string &filePath="./img/splash.png",
//                           const std::string &text="[PRESS Z OR ENTER]",
//                           sf::Vector2f positionText = sf::Vector2f(50, 400),
//                           unsigned int fontSize=24);
//     void show(sf::RenderWindow& window) const;
//     friend std::ostream & operator<<(std::ostream &os, const SplashScreen &obj);
// };