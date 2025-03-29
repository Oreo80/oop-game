#include "../Headers/BattleBox.h"

// void BattleBox::updateBounds() {
//     bounds = box.getGlobalBounds();
// }

BattleBox::BattleBox(const sf::Vector2f &pos, const sf::Vector2f &size) {
    box.setSize(size);
    box.setPosition(pos);
    box.setFillColor(sf::Color::Transparent);
    box.setOutlineColor(sf::Color::White);
    box.setOutlineThickness(5);
    // updateBounds();
};
std::ostream & operator<<(std::ostream &os, const BattleBox &obj) {
    return os
            << "Size: (" << obj.getSize().x << ", " << obj.getSize().y<<") "
            << ", Position: ("<< obj.getPosition().x << ", " << obj.getPosition().y<<") ";
}
// void BattleBox::resize(const sf::Vector2f& newSize) {
//     box.setSize(newSize);
// }
void BattleBox::draw(sf::RenderWindow& window) const {
    window.draw(box);
}

sf::FloatRect BattleBox::getGlobalBounds() const {
    return box.getGlobalBounds();
}


sf::Vector2f BattleBox::getSize() const {
    return box.getSize();
}

sf::Vector2f BattleBox::getPosition() const {
    return box.getPosition();
}
void BattleBox::setPosition(const sf::Vector2f &pos) {
    box.setPosition(pos);
}

float BattleBox::getOutlineThickness() const {
    return box.getOutlineThickness();
}

sf::Vector2f BattleBox::getCenter() const {
    return box.getPosition() + (box.getSize() / 2.f);
}
sf::FloatRect BattleBox::getInnerBounds() const {
    const sf::Vector2f rectPos = box.getPosition();
    const sf::Vector2f rectSize = box.getSize();
    const float thickness = box.getOutlineThickness();

    return {{rectPos.x + thickness, rectPos.y + thickness },
        {rectSize.x - 2 * thickness, rectSize.y - 2 * thickness}};
}
void BattleBox::setBottomY(const float fixedBottomY) {
    const float battleboxHeight = box.getSize().y;
    float const newBattleboxY = fixedBottomY - battleboxHeight;
    box.setPosition({box.getPosition().x, newBattleboxY});
}
void BattleBox::resizeCentered(const sf::Vector2f &deltaSize) {
    const sf::Vector2f currentSize = box.getSize();
    const sf::Vector2f currentPos = box.getPosition();
    const sf::Vector2f newSize = {currentSize.x + deltaSize.x, currentSize.y + deltaSize.y};
    const float bottomY = currentPos.y + currentSize.y;
    float newY = bottomY - newSize.y;

    box.setSize(newSize);
    box.setPosition({currentPos.x - (deltaSize.x / 2.f), newY});
}



