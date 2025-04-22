#include "../Headers/ShapeEntity.h"

ShapeEntity::ShapeEntity(const sf::Vector2f &pos, const sf::Vector2f &size) {
    shape.setSize(size);
    shape.setPosition(pos);
}

void ShapeEntity::draw(sf::RenderWindow &window) const {
    window.draw(shape);
}

sf::Vector2f ShapeEntity::getPosition() const {
    return shape.getPosition();
}

sf::Vector2f ShapeEntity::getSize() const {
    return shape.getSize();
}

sf::FloatRect ShapeEntity::getGlobalBounds() const {
    return shape.getGlobalBounds();
}

void ShapeEntity::setPosition(const sf::Vector2f &pos) {
    shape.setPosition(pos);
}

float ShapeEntity::getOutlineThickness() const {
    return shape.getOutlineThickness();
}

sf::Vector2f ShapeEntity::getCenter() const {
    return shape.getPosition() + (shape.getSize() / 2.f);
}

std::ostream & operator<<(std::ostream &os, const ShapeEntity &obj) {
    return os
        << "Size: (" << obj.getSize().x << ", " << obj.getSize().y<<") "
        << ", Position: ("<< obj.getPosition().x << ", " << obj.getPosition().y<<") ";
}
