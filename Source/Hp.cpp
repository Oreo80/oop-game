#include "../Headers/Hp.h"

void Hp::print(std::ostream &os) const {
    os << "HP: " << currentHp << " / " << maxHp;
}

Hp::Hp(const int maxHp_): maxHp(maxHp_), currentHp(maxHp),
                          hpLabelText("./fonts/fnt_small.png", "./fonts/glyphs_fnt_small.csv","HP",{244,405},sf::Color::White, 2.f),
                          hpValueText("./fonts/fnt_small.png", "./fonts/glyphs_fnt_small.csv",
                                      std::to_string(currentHp) + " / " + std::to_string(maxHp),{314,403},sf::Color::White, 3.f) {
    hpBar.setHealthBar(maxHp);
}

void Hp::takeDamage(const int dmg) {
    setHp(currentHp - dmg);
}

// void Hp::heal(const int amt) {
//     setHp(currentHp + amt);
// }

int Hp::getHp() const {
    return currentHp;
}

void Hp::setHp(const int newHp) {
    currentHp = newHp;
    hpBar.setHealthBar(newHp);
    hpValueText.setText(std::to_string(newHp) + " / " + std::to_string(maxHp));
}

void Hp::draw(sf::RenderWindow &window) const {
    hpLabelText.draw(window);
    hpValueText.draw(window);
    hpBar.draw(window);
}

sf::Vector2f Hp::getPosition() const {
    return hpBar.getPosition();
}

void Hp::setPosition(const sf::Vector2f &pos) {
    hpLabelText.setPosition(pos);
    hpValueText.setPosition(pos);
    hpBar.setPosition(pos);
}

sf::FloatRect Hp::getGlobalBounds() const {
    return hpBar.getGlobalBounds();
}

std::unique_ptr<DrawableEntity> Hp::clone() const {
    return std::make_unique<Hp>(*this);
}

std::ostream & operator<<(std::ostream &os, const Hp &obj) {
    obj.print(os);
    return os;
}
