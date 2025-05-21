#pragma once
#include "DrawableEntity.h"
#include "BitmapFont.h"
#include "ShapeEntity.h"
#include <SFML/Graphics.hpp>
#include <algorithm>

class Hp : public DrawableEntity {
private:
    int maxHp;
    int currentHp;
    BitmapFont hpLabelText;
    BitmapFont hpValueText;
    ShapeEntity backgroundBar;
    ShapeEntity fillBar;

protected:
    void print(std::ostream& os) const override;

public:
    explicit Hp(int maxHp_ = 20);
    void setHp( int newHp);
    void takeDamage(int dmg);
    int getHp() const;
    void draw(sf::RenderWindow &window) const override;
    sf::Vector2f getPosition() const override;
    void setPosition(const sf::Vector2f &pos) override;
    sf::FloatRect getGlobalBounds() const override;
    std::unique_ptr<DrawableEntity> clone() const override;
};
