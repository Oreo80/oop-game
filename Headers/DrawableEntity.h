#pragma once
#include <SFML/Graphics.hpp>


class DrawableEntity {
protected:
    virtual void print(std::ostream& os) const = 0;
public:
    void tick (sf::RenderWindow& window) {
        update();
        draw(window);
    }
    // nu prea pot sa fac update si draw protected din cauza la HP
    virtual void update() { } // no-op
    virtual void draw(sf::RenderWindow &window) const = 0;
    [[nodiscard]] virtual sf::Vector2f getPosition() const = 0;
    virtual void setPosition(const sf::Vector2f& pos) = 0;
    [[nodiscard]] virtual sf::FloatRect getGlobalBounds() const = 0;
    [[nodiscard]] virtual std::unique_ptr<DrawableEntity> clone() const = 0;
    virtual ~DrawableEntity() = default;
    friend std::ostream& operator<<(std::ostream &os, const DrawableEntity& obj) {
        obj.print(os);
        return os;
    }

};

