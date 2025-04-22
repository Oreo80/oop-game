#pragma once
#include "ShapeEntity.h"

class BattleBox : public ShapeEntity {
private:
    sf::Vector2f targetSize;
    bool isResizing = false;
public:
    BattleBox(const sf::Vector2f &pos, const sf::Vector2f &size) : ShapeEntity(pos, size) {
        shape.setFillColor(sf::Color::Transparent);
        shape.setOutlineColor(sf::Color::White);
        shape.setOutlineThickness(5);
    }
    float getBottomY() const;
    sf::FloatRect getInnerBounds() const;
    void setBottomY(float fixedBottomY);
    void resizeCentered(const sf::Vector2f& deltaSize);
    void updateResize(float pixelSpeed);
    bool getisResizing() const;
    friend std::ostream & operator<<(std::ostream &os, const BattleBox &obj);
};
