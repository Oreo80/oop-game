#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class SplashScreen {
private:
    sf::Texture splashTexture;
    sf::Sprite splashSprite;
    sf::Font font;
    sf::Text pressKeyText;
public:
    explicit SplashScreen()
    : splashTexture([&]() {
            sf::Texture tex;
            if (!tex.loadFromFile("./img/splash.png")) {
                std::cerr << "Failed to load splash texture!\n";
                throw std::runtime_error("Failed to load splash texture");
            }
            return tex;
        }()),
    splashSprite(splashTexture),
    font([&]() {
        sf::Font font_;
        if (!font_.openFromFile("./fonts/Roboto.ttf")) {
            std::cerr << "Failed to load font file!\n";
            throw std::runtime_error("Failed to load font file");
        }
        return font_;
    }()),
    pressKeyText(font, "[PRESS Z OR ENTER]", 24)
    {
        pressKeyText.setFillColor(sf::Color::White);
        pressKeyText.setPosition({50, 400});
        splashSprite.setPosition({0, 0});
    }
    void show(sf::RenderWindow& window) const;
    friend std::ostream & operator<<(std::ostream &os, const SplashScreen &obj);
};