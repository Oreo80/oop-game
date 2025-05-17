#include "../Headers/ShapeEntity.h"

void ShapeEntity::print(std::ostream &os) const {
    os << "Size: (" << getSize().x << ", " << getSize().y<<") "
        << ", Position: ("<< getPosition().x << ", " << getPosition().y<<") ";
}

ShapeEntity::ShapeEntity(const sf::Vector2f &pos, const sf::Vector2f &size) {
    shape.setSize(size);
    shape.setPosition(pos);
}

std::unique_ptr<DrawableEntity> ShapeEntity::clone() const {
    return std::make_unique<ShapeEntity>(*this);
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
    obj.print(os);
    return os;
}
