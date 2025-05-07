#include "../Headers/Player.h"

Player::Player(const std::string &normalPath, const std::string &hurtPath, const sf::Vector2f &pos): StatefulSprite(
    normalPath, "normal", pos) {
    addState("hurt", hurtPath);
}

// std::unique_ptr<DrawableEntity> Player::clone() const {
//     return std::make_unique<Player>(*this);
// }

void Player::move(const sf::Vector2f& offset){
    const sf::Vector2f newPosition=sprite.getPosition()+offset;
    sprite.setPosition(newPosition);
}

void Player::centerPlayer(const BattleBox &battleBox) {
    setOrigin({getGlobalBounds().size.x/2.f,getGlobalBounds().size.y/2.f});
    setPosition(battleBox.getCenter());
    setOrigin({0,0});
}

std::ostream & operator<<(std::ostream &os, const Player &obj){
    return os
            << static_cast<const StatefulSprite&>(obj);
}
