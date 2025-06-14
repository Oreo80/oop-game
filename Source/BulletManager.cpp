#include "../Headers/BulletManager.h"
#include <algorithm>


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
        bullet->update();
        bullet->draw(window);
    }
}

bool BulletManager::isEmpty() const {
    return bullets.empty();
}

void BulletManager::spawnFlyBullets(const int count, const float startY, const float spacing, sf::Vector2f speed) {
    for (int i = 0; i < count; ++i) {
        addBullet(std::make_unique<Bullet>(
            BulletID::Fly,
            sf::Vector2f{100.f, startY + (i + 1) * spacing},
            speed
        ));
    }
}