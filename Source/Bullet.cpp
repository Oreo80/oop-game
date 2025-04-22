#include "../Headers/Bullet.h"

void Bullet::update() {
    setPosition(getPosition() + velocity);
    updateAnimation();
}
bool Bullet::isOffScreen(const sf::RenderWindow& window) const {
    const float windowWidth = static_cast<float>(window.getSize().x);
    const float windowHeight = static_cast<float>(window.getSize().y);

    return getPosition().x < 0 || getPosition().x > windowWidth ||
           getPosition().y < 0 || getPosition().y > windowHeight;
}
std::ostream & operator<<(std::ostream &os, const Bullet &obj) {
    return os
        << static_cast<const AnimatedEntity&>(obj)
        << ", Velocity: ( "<< obj.velocity.x << ", " << obj.velocity.y
        << " , " << BulletType::get(obj.id);

}
