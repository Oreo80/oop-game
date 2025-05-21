#include "../Headers/Hp.h"

void Hp::print(std::ostream &os) const {
    os << "HP: " << currentHp << " / " << maxHp;
}

Hp::Hp(const int maxHp_): maxHp(maxHp_), currentHp(maxHp_),
                          hpLabelText("./fonts/fnt_small.png", "./fonts/glyphs_fnt_small.csv",
                                      "HP", {244, 405}, sf::Color::White, 2.f),
                          hpValueText("./fonts/fnt_small.png", "./fonts/glyphs_fnt_small.csv",
                                      std::to_string(maxHp_) + " / " + std::to_string(maxHp_),
                                      {314, 403}, sf::Color::White, 3.f),
                          backgroundBar({275,400}, {25,21}),
                          fillBar({275,400}, {25,21}) {
    backgroundBar.setFillColor(sf::Color::Red);
    fillBar.setFillColor(sf::Color::Yellow);
}

void Hp::setHp(const int newHp) {
    currentHp = std::clamp(newHp, 0, maxHp);
    const float ratio = static_cast<float>(currentHp) / maxHp;
    auto bgSize = backgroundBar.getSize();
    fillBar.setSize({bgSize.x * ratio, bgSize.y});
    hpValueText.setText(std::to_string(currentHp) + " / " + std::to_string(maxHp));
}

void Hp::takeDamage(const int dmg) {
    setHp(currentHp - dmg);
}

int Hp::getHp() const { return currentHp; }

void Hp::draw(sf::RenderWindow &window) const {
    hpLabelText.draw(window);
    hpValueText.draw(window);
    backgroundBar.draw(window);
    fillBar.draw(window);
}

sf::Vector2f Hp::getPosition() const {
    return backgroundBar.getPosition();
}

void Hp::setPosition(const sf::Vector2f &pos) {
    backgroundBar.setPosition(pos);
    fillBar.setPosition(pos);
    constexpr sf::Vector2f labelOffset = {-31.f, 5.f};
    constexpr sf::Vector2f valueOffset = {39.f, 3.f};
    hpLabelText.setPosition(pos + labelOffset);
    hpValueText.setPosition(pos + valueOffset);
}

sf::FloatRect Hp::getGlobalBounds() const {
    return backgroundBar.getGlobalBounds();
}

std::unique_ptr<DrawableEntity> Hp::clone() const {
    return std::make_unique<Hp>(*this);
}
