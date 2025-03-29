#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Button {
private:
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Vector2f position;

public:
    explicit Button(const std::string& texturePath="./img/missing.png", const sf::Vector2f& pos={0,0})
        : texture([&]() {
            sf::Texture tex;
            if (!tex.loadFromFile(texturePath)) {
                std::cerr << "Failed to load texture! "<<texturePath<<std::endl;
                throw std::runtime_error("Failed to load texture "+ texturePath);
            }
            return tex;
        }()),
          sprite(texture),
          position(pos)
    {
        sprite.setPosition(position);
    }

    friend std::ostream & operator<<(std::ostream &os, const Button &obj);

    void draw(sf::RenderWindow& window) const;

};