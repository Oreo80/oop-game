#pragma once
#include <SFML/Graphics.hpp>


class DrawableEntity {
    public:
    void tick (sf::RenderWindow& window) {
        update();
        draw(window);
    }
    virtual void update() { } // no-op
    virtual void draw(sf::RenderWindow &window) const = 0;
    [[nodiscard]] virtual sf::Vector2f getPosition() const = 0;
    virtual void setPosition(const sf::Vector2f& pos) = 0;
    [[nodiscard]] virtual sf::FloatRect getGlobalBounds() const = 0;
    // [[nodiscard]] virtual std::unique_ptr<DrawableEntity> clone() const = 0;
    virtual ~DrawableEntity() = default;

};

