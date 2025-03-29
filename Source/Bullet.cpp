#include "../Headers/Bullet.h"

const BulletType& Bullet::getDefaultBulletType() {
    static const BulletType defaultbullet;
    return defaultbullet;
}

Bullet::Bullet(const BulletType& type, const sf::Vector2f& startPos, const sf::Vector2f dir)
: bulletType(type),position(startPos), direction(dir), velocity(direction * type.getSpeed()), sprite(type.getTextures()[0])
{
    sprite.setPosition(position);
}

Bullet::Bullet(const Bullet &other) = default;

Bullet::Bullet(Bullet &&other) noexcept
    : currentFrame(other.currentFrame),
      animationClock(other.animationClock),
      bulletType(std::move(other.bulletType)),
      position(other.position),
      velocity(other.velocity),
      sprite(std::move(other.sprite)) {
}

Bullet & Bullet::operator=(const Bullet &other) {
    if (this == &other)
        return *this;
    currentFrame = other.currentFrame;
    animationClock = other.animationClock;
    bulletType = other.bulletType;
    position = other.position;
    velocity = other.velocity;
    sprite = other.sprite;
    return *this;
}

Bullet & Bullet::operator=(Bullet &&other) noexcept {
    if (this == &other)
        return *this;
    currentFrame = other.currentFrame;
    animationClock = other.animationClock;
    bulletType = std::move(other.bulletType);
    position = other.position;
    velocity = other.velocity;
    sprite = std::move(other.sprite);
    return *this;
}

Bullet::~Bullet() = default;

std::ostream & operator<<(std::ostream &os, const Bullet &obj) {
    return os
        << "Position: (" << obj.position.x << ", " << obj.position.y << ") "
        << ", Velocity: ( "<< obj.velocity.x << ", " << obj.velocity.y
        << " , Current Frame: " << obj.currentFrame
        << " , BulletType: " << obj.bulletType;

}

void Bullet::update() {
    position += velocity;
    sprite.setPosition(position);

    if (animationClock.getElapsedTime().asMilliseconds() > 100) {
        currentFrame = (currentFrame + 1) % bulletType.getTextures().size();
        sprite.setTexture(bulletType.getTextures()[currentFrame]);
        animationClock.restart();
    }
}

void Bullet::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
}

bool Bullet::isOffScreen(const sf::RenderWindow& window) const {
    const float windowWidth = static_cast<float>(window.getSize().x);
    const float windowHeight = static_cast<float>(window.getSize().y);

    return position.x < 0 || position.x > windowWidth ||
           position.y < 0 || position.y > windowHeight;
}
sf::Vector2f Bullet::getPosition() const {
    return position;
}
sf::FloatRect Bullet::getGlobalBounds() const {
    return sprite.getGlobalBounds();
}