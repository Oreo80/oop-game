#include "../Headers/BattleBox.h"
#include <algorithm>

float BattleBox::getBottomY() const {
    return shape.getPosition().y + shape.getSize().y;
}
sf::FloatRect BattleBox::getInnerBounds() const {
    const sf::Vector2f rectPos = shape.getPosition();
    const sf::Vector2f rectSize = shape.getSize();
    const float thickness = shape.getOutlineThickness();

    return {{rectPos.x + thickness, rectPos.y + thickness },
        {rectSize.x - 2 * thickness, rectSize.y - 2 * thickness}};
}

void BattleBox::setBottomY(const float fixedBottomY) {
    const float currentBottomY = getBottomY();
    const float yDelta = fixedBottomY - currentBottomY;
    shape.setPosition({
        shape.getPosition().x,
        shape.getPosition().y + yDelta
    });
}

void BattleBox::resizeCentered(const sf::Vector2f &deltaSize) {
    targetSize = {shape.getSize().x + deltaSize.x, shape.getSize().y + deltaSize.y};
    isResizing = true;
}

void BattleBox::updateResize(const float pixelSpeed) {
    const sf::Vector2f currentSize = shape.getSize();
    const sf::Vector2f sizeDiff = targetSize - currentSize;

    const sf::Vector2f step{
        std::clamp(sizeDiff.x, -pixelSpeed, pixelSpeed),
        std::clamp(sizeDiff.y, -pixelSpeed, pixelSpeed)
    };

    sf::Vector2f newSize = currentSize + step;

    if (std::abs(sizeDiff.x) < pixelSpeed && std::abs(sizeDiff.y) < pixelSpeed) {
        newSize = targetSize;
        isResizing = false;
    }

    const sf::Vector2f currentPos = shape.getPosition();
    const float bottomY = getBottomY();
    float newY = bottomY - newSize.y;

    shape.setSize(newSize);
    shape.setPosition({currentPos.x - step.x / 2.f, newY});
}

std::ostream & operator<<(std::ostream &os, const BattleBox &obj){
    return os
            << static_cast<const ShapeEntity&>(obj);
}

