#include "../Headers/Bullet.h"

Bullet::Bullet(const BulletID id_, const sf::Vector2f &startPos, const sf::Vector2f &dir): AnimatedEntity(
        BulletType::get(id_).getPaths(), startPos),
    id(id_),
    direction(dir),
    velocity(dir * BulletType::get(id_).getSpeed()) {
}

// std::unique_ptr<DrawableEntity> Bullet::clone() const {
//     return std::make_unique<Bullet>(*this);
// }

void Bullet::update() {
    setPosition(getPosition() + velocity);
    animate();
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
