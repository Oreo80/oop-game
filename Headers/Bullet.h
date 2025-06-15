#pragma once
#include "BulletType.h"
#include "AnimatedEntity.h"

class Bullet : public AnimatedEntity {
private:
    BulletID id;
    sf::Vector2f direction;
    sf::Vector2f velocity;
    void swap(Bullet& other) noexcept;
protected:
    void print(std::ostream& os) const override;
public:
    explicit Bullet(BulletID id_ = BulletID::Fly,
                    const sf::Vector2f &startPos = {100.f, 100.f},
                    const sf::Vector2f &dir = {1.0f, 0.0f});
    Bullet(const Bullet& other) = default;
    Bullet& operator=(Bullet other);
    std::unique_ptr<DrawableEntity> clone() const override;
    void update() override;
    bool isOffScreen(const sf::RenderWindow& window) const;
};
