#include "../Headers/Bullet.h"

void Bullet::swap(Bullet &other) noexcept {
    using std::swap;
    AnimatedEntity::operator=(static_cast<AnimatedEntity>(other));
    swap(id, other.id);
    swap(direction, other.direction);
    swap(velocity, other.velocity);
}

void Bullet::print(std::ostream &os) const {
    AnimatedEntity::print(os);
        os << ", Velocity: ( "<< velocity.x << ", " << velocity.y
        << " , " << BulletType::get(id);
}

Bullet::Bullet(const BulletID id_, const sf::Vector2f &startPos, const sf::Vector2f &dir): AnimatedEntity(
                                                                                               BulletType::get(id_).getPaths(), startPos),
                                                                                           id(id_),
                                                                                           direction(dir),
                                                                                           velocity(dir * BulletType::get(id_).getSpeed()) {
}

Bullet & Bullet::operator=(Bullet other) {
    swap(other);
    return *this;
}

std::unique_ptr<DrawableEntity> Bullet::clone() const {
    return std::make_unique<Bullet>(*this);
}

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