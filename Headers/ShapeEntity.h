#pragma once
#include "DrawableEntity.h"

class ShapeEntity : public DrawableEntity {
protected:
    sf::RectangleShape shape;
public:
    ShapeEntity(const sf::Vector2f &pos, const sf::Vector2f &size);
    // std::unique_ptr<DrawableEntity> clone() const override;
    void draw(sf::RenderWindow &window) const override;
    sf::Vector2f getPosition() const override;
    sf::Vector2f getSize() const;
    sf::FloatRect getGlobalBounds() const override;
    void setPosition(const sf::Vector2f &pos) override;
    float getOutlineThickness() const;
    sf::Vector2f getCenter() const;

    ~ShapeEntity() override = default;
    friend std::ostream & operator<<(std::ostream &os, const ShapeEntity &obj);
};