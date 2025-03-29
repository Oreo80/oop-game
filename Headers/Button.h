#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <utility>

class Button {
private:
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Vector2f position;
    std::string normalTexturePath;
    std::string selectedTexturePath;
public:
    explicit Button(std::string normalPath = "./img/missing.png",
                    std::string selectedPath = "./img/missing.png", const sf::Vector2f &pos = {0, 0})
        : texture([&]() {
            sf::Texture tex;
            if (!tex.loadFromFile(normalPath)) {
                std::cerr << "Failed to load texture! "<<normalPath<<std::endl;
                throw std::runtime_error("Failed to load texture "+ normalPath);
            }
            return tex;
        }()),
          sprite(texture),
          position(pos),
          normalTexturePath(std::move(normalPath)),
          selectedTexturePath(std::move(selectedPath))
    {
        sprite.setPosition(position);
    }

    friend std::ostream & operator<<(std::ostream &os, const Button &obj);

    void draw(sf::RenderWindow& window) const;
    void setTexture(bool isSelected);
    sf::Vector2f getPositionForPlayer() const;

};