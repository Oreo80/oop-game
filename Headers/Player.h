#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Player {
private:
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Vector2f position;

public:
    explicit Player(const std::string& texturePath="./img/spr_heart_0.png", const sf::Vector2f& pos={312, 232})
        : texture([&]() {
            sf::Texture tex;
            if (!tex.loadFromFile(texturePath)) {
                std::cerr << "Failed to load sprite texture!\n";
                throw std::runtime_error("Failed to load sprite texture");
            }
            return tex;
        }()),
          sprite(texture),
          position(pos)
            {
              sprite.setPosition(position);
            }

    friend std::ostream & operator<<(std::ostream &os, const Player &obj);

    void draw(sf::RenderWindow& window) const;
    void setPosition(const sf::Vector2f& position_);

    sf::Vector2f getPosition() const;
    void move(const sf::Vector2f& offset);
    sf::FloatRect getGlobalBounds() const;
};