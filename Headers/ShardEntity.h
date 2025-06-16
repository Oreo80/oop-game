#pragma once
#include "AnimatedEntity.h"

class ShardEntity : public AnimatedEntity {
    sf::Vector2f velocity;
    float gravity = 0.2f;

public:
    ShardEntity(const std::vector<std::string>& paths, const sf::Vector2f& pos, const sf::Vector2f& initialVelocity);

    ShardEntity(const ShardEntity &other);
    ShardEntity(ShardEntity &&other) noexcept;
    ShardEntity & operator=(ShardEntity other);
    void update() override;
    // cppcheck-suppress unusedFunction
    std::unique_ptr<DrawableEntity> clone() const override;

};