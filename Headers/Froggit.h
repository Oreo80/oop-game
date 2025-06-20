#pragma once
#include "Enemy.h"
#include <cmath>

class Froggit : public Enemy {
private:
    std::vector<std::shared_ptr<sf::Texture>> headFrames;
    sf::Sprite headSprite;
    sf::Clock headAnimationClock;
    sf::Clock moveClock;
    int headCycleTime = 2000;
    size_t currentHeadFrame = 0;

    float amplitude = 2.5f;
    float speedX = 5.0f;
    float speedY = 5.5f;
    float phaseOffset = 1.0f;

public:
    explicit Froggit(const sf::Vector2f& pos = {215, 135});
    void update() override;
    void draw(sf::RenderWindow& window) const override;
    [[nodiscard]] std::unique_ptr<Enemy> cloneEnemy() const override;
};