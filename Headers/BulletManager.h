#pragma once

#include "Bullet.h"
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

class BulletManager {
private:
    std::vector<std::unique_ptr<Bullet>> bullets;
public:
    void cleanup(const sf::RenderWindow& window);
    void clearBullets();
    void addBullet(std::unique_ptr<Bullet> bullet);
    void tick(sf::RenderWindow& window);
    std::vector<std::unique_ptr<Bullet>>& getBullets() { return bullets; }
    bool isEmpty() const;
    void spawnFlyBullets(int count, float startY, float spacing, sf::Vector2f speed);
};