#include "../Headers/Player.h"

std::ostream & operator<<(std::ostream &os, const Player &obj){
  return os
          << "Position: (" << obj.position.x << ", " << obj.position.y << ")";
}

void Player::draw(sf::RenderWindow& window) const{
    window.draw(sprite);
}

void Player::setPosition(const sf::Vector2f& position_){
    sprite.setPosition(position_);
}

sf::Vector2f Player::getPosition() const {
    return sprite.getPosition();
}

void Player::move(const sf::Vector2f& offset){
    position += offset;
    sprite.setPosition(position);
}

sf::FloatRect Player::getGlobalBounds() const {
    return sprite.getGlobalBounds();
}