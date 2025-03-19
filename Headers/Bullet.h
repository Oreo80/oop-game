#pragma once
#include "BulletType.h"
#include <iostream>

class Bullet {
private:
    size_t currentFrame = 0;
    sf::Clock animationClock;
    BulletType bulletType;
    sf::Vector2f position;
    sf::Vector2f direction;
    sf::Vector2f velocity;
    sf::Sprite sprite;

    static const BulletType& getDefaultBulletType();
public:
    explicit Bullet(const BulletType& type=getDefaultBulletType(), const sf::Vector2f& startPos={100.f,100.f}, sf::Vector2f dir={ 1.0f, 0.0f });

    Bullet(const Bullet &other);

    Bullet(Bullet &&other) noexcept;

    Bullet & operator=(const Bullet &other);

    Bullet & operator=(Bullet &&other) noexcept;
    ~Bullet();

    friend std::ostream & operator<<(std::ostream &os, const Bullet &obj);

    void update();

    void draw(sf::RenderWindow& window) const;

    bool isOffScreen(const sf::RenderWindow& window) const;
    sf::Vector2f getPosition() const;
    sf::FloatRect getGlobalBounds() const;
};
