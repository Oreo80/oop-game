#include "../Headers/HpBar.h"

HpBar::HpBar(const sf::Vector2f &pos, const sf::Vector2f &size) :ShapeEntity(pos, size) {
    health.setPosition(pos);
    health.setSize(size);
    shape.setFillColor(sf::Color::Red);
    health.setFillColor(sf::Color::Yellow);
}

void HpBar::setHealthBar(const int healthValue) {
    const float damagepx = shape.getSize().x / 20;
    health.setSize({static_cast<float>(healthValue)*damagepx,health.getSize().y});
}

std::unique_ptr<DrawableEntity> HpBar::clone() const {
    return std::make_unique<HpBar>(*this);
}

void HpBar::draw(sf::RenderWindow &window) const {
    window.draw(shape);
    window.draw(health);
}

void HpBar::setPosition(const sf::Vector2f &pos) {
    shape.setPosition(pos);
    health.setPosition(pos);
}

std::ostream & operator<<(std::ostream &os, const HpBar &obj) {
    obj.print(os);
    return os;
}