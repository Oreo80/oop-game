#pragma once
#include "HpBar.h"
#include "BitmapFont.h"

class Hp : public DrawableEntity {
private:
    int maxHp;
    int currentHp;
    BitmapFont hpLabelText;
    BitmapFont hpValueText;
    HpBar hpBar;
protected:
    void print(std::ostream& os) const override;
public:
    explicit Hp(int maxHp_ = 20);
    void setHp(int newHp);
    void takeDamage(int dmg);
    void heal(int amt);
    int getHp() const;
    void draw(sf::RenderWindow &window) const override;
    sf::Vector2f getPosition() const override;
    void setPosition(const sf::Vector2f &pos) override;
    sf::FloatRect getGlobalBounds() const override;
    std::unique_ptr<DrawableEntity> clone() const override;
    friend std::ostream& operator<<(std::ostream &os, const Hp& obj);
};