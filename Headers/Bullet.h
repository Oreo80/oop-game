#pragma once
#include "BulletType.h"
#include "AnimatedEntity.h"

class Bullet : public AnimatedEntity {
private:
    BulletID id;
    sf::Vector2f direction;
    sf::Vector2f velocity;
public:
    explicit Bullet(const BulletID id_ = BulletID::Fly,
                    const sf::Vector2f &startPos = {100.f, 100.f},
                    const sf::Vector2f &dir = {1.0f, 0.0f})
                    : AnimatedEntity(BulletType::get(id_).getPaths(), startPos),
                    id(id_),
                    direction(dir),
                    velocity(dir * BulletType::get(id_).getSpeed()) {
    }
    void update();
    bool isOffScreen(const sf::RenderWindow& window) const;

    friend std::ostream & operator<<(std::ostream &os, const Bullet &obj);
};
