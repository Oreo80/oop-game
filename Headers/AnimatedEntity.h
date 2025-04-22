#pragma once
#include <iostream>

#include "DrawableEntity.h"
#include "TextureCache.h"

class AnimatedEntity : public DrawableEntity {
protected:
    std::vector<std::shared_ptr<sf::Texture>> frames;
    sf::Sprite sprite;
    std::size_t currentFrame = 0;
    sf::Clock animationClock;

    static std::vector<std::shared_ptr<sf::Texture>> loadTextures(const std::vector<std::string>& paths);
public:
    explicit AnimatedEntity(const std::vector<std::string> &paths,
                               const sf::Vector2f &pos) : frames(loadTextures(paths)), sprite(*frames[0]) {
        sprite.setPosition(pos);
    }
    ~AnimatedEntity() override = default;

    void updateAnimation();

    void draw(sf::RenderWindow& window) const override;
    sf::Vector2f getPosition() const override;
    void setPosition(const sf::Vector2f& pos) override;
    sf::FloatRect getGlobalBounds() const override;

    friend std::ostream & operator<<(std::ostream &os, const AnimatedEntity &obj);

};