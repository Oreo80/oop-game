#include "../Headers/Player.h"

void Player::updateHurtAnimation() {
    if (hurtFramesRemaining <= 0) return;
    int phaseIndex = ((hurtFramesRemaining - 1) / framesPerPhase) % 2;
    if (phaseIndex == 1) {
        setState("hurt");
    } else {
        setState("normal");
    }

    --hurtFramesRemaining;

    if (hurtFramesRemaining == 0) {
        setState("normal");
    }
}

Player::Player(const std::string &normalPath, const std::string &hurtPath, const sf::Vector2f &pos): StatefulSprite(
    normalPath, "normal", pos) {
    addState("hurt", hurtPath);
}

std::unique_ptr<DrawableEntity> Player::clone() const {
    return std::make_unique<Player>(*this);
}

void Player::move(const sf::Vector2f& offset){
    const sf::Vector2f newPosition=sprite.getPosition()+offset;
    sprite.setPosition(newPosition);
}

void Player::centerPlayer(const BattleBox &battleBox) {
    setOrigin({getGlobalBounds().size.x/2.f,getGlobalBounds().size.y/2.f});
    setPosition(battleBox.getCenter());
    setOrigin({0,0});
}

void Player::startHurtAnimation() {
    hurtFramesRemaining = 2 * framesPerPhase * repeatCount;
}

bool Player::isHurting() const {
    return hurtFramesRemaining > 0;
}

void Player::update() {
    updateHurtAnimation();
}

std::ostream & operator<<(std::ostream &os, const Player &obj){
    return os
            << static_cast<const StatefulSprite&>(obj);
}
