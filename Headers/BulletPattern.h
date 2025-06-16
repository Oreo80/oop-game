#pragma once
#include <vector>
#include <memory>
#include <cmath>
#include "BulletManager.h"
#include "Bullet.h"

class SineBullet : public Bullet {
    sf::Vector2f startPos;
    float amplitude;
    float frequency;
    float phase;
    float speed;
    int frameCount = 0;
public:
    SineBullet(const BulletID id, const sf::Vector2f start,float spd, const float amp, const float freq, const float ph)
        : Bullet(id, start, {spd, 0.f}),
          startPos(start), amplitude(amp), frequency(freq), phase(ph), speed(spd)
    {}

    void update() override {
        frameCount++;
        sf::Vector2f p = getPosition();
        p.x += speed;
        p.y = startPos.y + amplitude * std::sin(frequency * frameCount + phase);
        setPosition(p);
        animate();
    }
};

class MovingBullet : public Bullet {
    sf::Vector2f dir;
    float speed;
    sf::Vector2f center;
    sf::Vector2f halfExtents;
    bool forward = true;
public:
    MovingBullet(const BulletID id, const sf::Vector2f start, const sf::Vector2f direction, const sf::Vector2f halfExt)
        : Bullet(id, start, direction),
          dir(direction),
          speed(std::sqrt(direction.x*direction.x + direction.y*direction.y)),
          center(start),
          halfExtents(halfExt)
    {}

    void update() override {
        sf::Vector2f p = getPosition();
        const float delta = forward ? speed : -speed;
        p += dir * delta;
        if (p.x < center.x - halfExtents.x || p.x > center.x + halfExtents.x ||
            p.y < center.y - halfExtents.y || p.y > center.y + halfExtents.y) {
            forward = !forward;
        }
        setPosition(p);
        animate();
    }
};

namespace BulletPatterns {

inline void spawnTriangleSpread(BulletManager& bm, const int count, const sf::Vector2f start, const sf::Vector2f baseOffset,
                                const float forwardOffset, const float speed) {
    for (int i = 0; i < count; ++i) {
        const int half = count / 2;
        const int offset = i - half;
        sf::Vector2f pos = start + baseOffset * static_cast<float>(offset) + sf::Vector2f{forwardOffset * std::abs(offset), 0.f};
        constexpr sf::Vector2f dir = {1.f, 0.f};
        bm.addBullet(std::make_unique<Bullet>(BulletID::Fly, pos, dir * speed));
    }
}

inline void spawnSineWave(BulletManager& bm, const int count, const sf::Vector2f startPos, const sf::Vector2f spacing,
                            float amplitude,float frequency,float speed) {
    for (int i = 0; i < count; ++i) {
        sf::Vector2f p = startPos + spacing * static_cast<float>(i);
        float phase = static_cast<float>(i) * 0.5f;
        bm.addBullet(std::make_unique<SineBullet>(BulletID::Fly, p, speed, amplitude, frequency, phase));
    }
}

    inline void spawnWindowPattern(BulletManager& bm, const sf::Vector2f center, sf::Vector2f halfExtents, sf::Vector2f direction) {
    std::vector<sf::Vector2f> offsets = {
        {-halfExtents.x, -halfExtents.y}, // top-left
        { halfExtents.x, -halfExtents.y}, // top-right
        {-halfExtents.x,  halfExtents.y}, // bottom-left
        { halfExtents.x,  halfExtents.y}, // bottom-right
        {0.f,            -halfExtents.y}, // top-center
        {0.f,             halfExtents.y}, // bottom-center
        {-halfExtents.x,  0.f},// middle-left
        { halfExtents.x,  0.f} // middle-right
    };
    for (const auto& offset : offsets) {
        bm.addBullet(std::make_unique<MovingBullet>(BulletID::Fly,
                                                    center + offset,
                                                    direction,
                                                    halfExtents));
    }
}

    inline void spawnFlyLine(BulletManager& bm, const int count, const sf::Vector2f start, const float spacing,
                            const sf::Vector2f direction, const float speed) {
    for (int i = 0; i < count; ++i) {
        sf::Vector2f pos = start + sf::Vector2f{0.f, i * spacing};
        bm.addBullet(std::make_unique<Bullet>(BulletID::Fly, pos, direction * speed));
    }
}

}
