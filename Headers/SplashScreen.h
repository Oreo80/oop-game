#pragma once
#include "SpriteEntity.h"

class SplashScreen : public SpriteEntity {
private:
    sf::Font font;
    sf::Text splashText;

    static sf::Font loadFont();
public:
    explicit SplashScreen(const std::string &filePath="./img/splash.png",
                          const std::string &text="[PRESS Z OR ENTER]",
                          const sf::Vector2f positionText = sf::Vector2f(50, 400),
                          const unsigned int fontSize=24)
                          : SpriteEntity(filePath, {0,0}),
                          font(loadFont()),
                          splashText(font, text, fontSize) {
        splashText.setFillColor(sf::Color::White);
        splashText.setPosition(positionText);
    }
    void show(sf::RenderWindow& window) const;
    friend std::ostream & operator<<(std::ostream &os, const SplashScreen &obj);
};
// #include <SFML/Graphics.hpp>
// #include <iostream>
//
// class SplashScreen {
// private:
//     sf::Texture splashTexture;
//     sf::Sprite splashSprite;
//     sf::Font font;
//     sf::Text splashText;
// public:
//     explicit SplashScreen(const std::string &filePath="./img/splash.png", const std::string &text="[PRESS Z OR ENTER]",
//         const sf::Vector2f positionText = sf::Vector2f(50, 400), const unsigned int fontSize=24)
//     : splashTexture([&]() {
//             sf::Texture tex;
//             if (!tex.loadFromFile(filePath)) {
//                 std::cerr << "Failed to load texture! "<< filePath << std::endl;
//                 throw std::runtime_error("Failed to load texture " + filePath);
//             }
//             return tex;
//         }()),
//     splashSprite(splashTexture),
//     font([&]() {
//         sf::Font font_;
//         if (!font_.openFromFile("./fonts/DeterminationSans.ttf")) {
//             std::cerr << "Failed to load font file!\n";
//             throw std::runtime_error("Failed to load font file");
//         }
//         return font_;
//     }()),
//     splashText(font, text, fontSize)
//     {
//         splashText.setFillColor(sf::Color::White);
//         splashText.setPosition(positionText);
//         splashSprite.setPosition({0, 0});
//     }
//     void show(sf::RenderWindow& window) const;
//     friend std::ostream & operator<<(std::ostream &os, const SplashScreen &obj);
// };