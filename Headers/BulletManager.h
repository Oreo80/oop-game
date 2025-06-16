#pragma once

#include "Bullet.h"
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

class BulletManager {
private:
    std::vector<std::unique_ptr<Bullet>> bullets;
public:
    void spawnRandomPattern(const sf::Vector2f& playerPosition);
    void cleanup(const sf::RenderWindow& window);
    void clearBullets();
    void addBullet(std::unique_ptr<Bullet> bullet);
    void tick(sf::RenderWindow& window);
    std::vector<std::unique_ptr<Bullet>>& getBullets() { return bullets; }
    bool isEmpty() const;
};