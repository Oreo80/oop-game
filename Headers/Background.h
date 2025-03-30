#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Background {
private:
    sf::Texture texture;
    sf::Sprite sprite;
    std::string texturePath;
public:
    explicit Background(const std::string &fontFilePath = "./img/missing.png")
        : texture([&]() {
            sf::Texture tex;
            if (!tex.loadFromFile(fontFilePath)) {
                std::cerr << "Failed to load texture! "<<fontFilePath<<std::endl;
                throw std::runtime_error("Failed to load texture "+ fontFilePath);
            }
            return tex;
        }()),
          sprite(texture),
          texturePath(fontFilePath)
    {
        sprite.setPosition({0,0});
    }

    friend std::ostream & operator<<(std::ostream &os, const Background &obj);

    void draw(sf::RenderWindow& window) const;
};