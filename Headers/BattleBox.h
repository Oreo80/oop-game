#pragma once
#include "ShapeEntity.h"

class BattleBox : public ShapeEntity {
private:
    sf::Vector2f targetSize;
    bool isResizing = false;
public:
    BattleBox(const sf::Vector2f &pos, const sf::Vector2f &size);
    std::unique_ptr<DrawableEntity> clone() const override;
    float getBottomY() const;
    sf::FloatRect getInnerBounds() const;
    void setBottomY(float fixedBottomY);
    void resizeCentered(const sf::Vector2f& deltaSize);
    void update() override;
};
