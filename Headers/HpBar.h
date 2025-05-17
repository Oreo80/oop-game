#pragma once
#include "ShapeEntity.h"

class HpBar : public ShapeEntity {
private:
    sf::RectangleShape health;
public:
    explicit HpBar(const sf::Vector2f &pos = {275,400}, const sf::Vector2f &size = {25,21});
    void setHealth(int healthValue);
    std::unique_ptr<DrawableEntity> clone() const override;
    void draw(sf::RenderWindow &window) const override;
    void setPosition(const sf::Vector2f &pos) override;
};