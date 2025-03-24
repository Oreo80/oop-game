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
//     updateBounds();
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
    return {box.getPosition().x+box.getSize().x/2, box.getPosition().y+box.getSize().y/2};
}