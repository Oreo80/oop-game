#include "../Headers/BulletManager.h"
#include "../Headers/BulletPattern.h"
#include <algorithm>
#include <random>


void BulletManager::spawnRandomPattern(const sf::Vector2f& playerPosition) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> distrib(0, 3);

    switch (distrib(gen)) {
        case 0:
            BulletPatterns::spawnTriangleSpread(
                *this,
                6,
                {playerPosition.x - 250, playerPosition.y},
                {0.f, 30.f},
                25.f,
                1.f
            );
        break;
        case 1:
            BulletPatterns::spawnSineWave(
                *this,
                7,
                {playerPosition.x - 350, playerPosition.y},
                {40.f, 0.f},
                40.f, 0.1f, 3.5f
            );
        break;
        case 2:
            BulletPatterns::spawnWindowPattern(
                *this,
                playerPosition,
                {50.f, 30.f},
                {1.f, 0.f}
            );
        break;
        case 3:
            BulletPatterns::spawnFlyLine(
                *this,
                6,
                {100.f, 225.f},
                30.f,
                {1.f, 0.f},
                1.f
            );
        break;
        default:
            break;
    }
}

void BulletManager::cleanup(const sf::RenderWindow& window) {
    std::erase_if(bullets,
                  [&](const auto& bullet) {
                      return bullet->isOffScreen(window);
                  });
}

void BulletManager::clearBullets() {
    bullets.clear();
}


void BulletManager::addBullet(std::unique_ptr<Bullet> bullet) {
    bullets.push_back(std::move(bullet));
}

void BulletManager::tick(sf::RenderWindow& window) {
    cleanup(window);
    for (const auto& bullet : bullets) {
        bullet->tick(window);
    }
}

bool BulletManager::isEmpty() const {
    return bullets.empty();
}