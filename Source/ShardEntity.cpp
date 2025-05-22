#include "../Headers/ShardEntity.h"

ShardEntity::ShardEntity(const std::vector<std::string> &paths, const sf::Vector2f &pos,
    const sf::Vector2f &initialVelocity): AnimatedEntity(paths, pos, 80), velocity(initialVelocity) {}

ShardEntity::ShardEntity(const ShardEntity &other): AnimatedEntity(other),
                                                    velocity(other.velocity),
                                                    gravity(other.gravity) {
}

ShardEntity::ShardEntity(ShardEntity &&other) noexcept: AnimatedEntity(std::move(other)),
                                                        velocity(std::move(other.velocity)),
                                                        gravity(other.gravity) {
}

ShardEntity & ShardEntity::operator=(ShardEntity other) {
    using std::swap;
    swap(*this, other);
    return *this;
}

void ShardEntity::update() {
    sprite.move(velocity);
    velocity.y += gravity;
    AnimatedEntity::update();
}

std::unique_ptr<DrawableEntity> ShardEntity::clone() const {
    return std::make_unique<ShardEntity>(*this);
}