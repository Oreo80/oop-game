#pragma once
#include <SFML/Graphics.hpp>
class BattleBox {
private:
    sf::RectangleShape box;
    sf::FloatRect bounds;

    void updateBounds();
public:
    BattleBox(const sf::Vector2f &pos, const sf::Vector2f &size);
    // void resize(const sf::Vector2f& newSize);
    void draw(sf::RenderWindow& window) const;
    sf::FloatRect getGlobalBounds() const;
    sf::Vector2f getSize() const;
    sf::Vector2f getPosition() const;
    float getOutlineThickness() const;
};