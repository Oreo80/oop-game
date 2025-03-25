#pragma once
#include <SFML/Graphics.hpp>
class BattleBox {
private:
    sf::RectangleShape box;
    // sf::FloatRect bounds;

    // void updateBounds();
public:
    BattleBox(const sf::Vector2f &pos, const sf::Vector2f &size);
    friend std::ostream & operator<<(std::ostream &os, const BattleBox &obj);
    // void resize(const sf::Vector2f& newSize);
    void draw(sf::RenderWindow& window) const;
    sf::FloatRect getGlobalBounds() const;
    sf::Vector2f getSize() const;
    sf::Vector2f getPosition() const;
    void setPosition(const sf::Vector2f &pos);
    float getOutlineThickness() const;
    sf::Vector2f getCenter() const;
    sf::FloatRect getInnerBounds() const;
    void setBottomY(float fixedBottomY);

};