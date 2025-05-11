#pragma once
#include "ResourceManager.h"
#include "DrawableEntity.h"

class AnimatedEntity : public DrawableEntity {
protected:
    std::vector<std::shared_ptr<sf::Texture>> frames;
    sf::Sprite sprite;
    std::size_t currentFrame = 0;
    sf::Clock animationClock;

    static std::vector<std::shared_ptr<sf::Texture>> loadTextures(const std::vector<std::string>& paths);

public:
    explicit AnimatedEntity(const std::vector<std::string> &paths, const sf::Vector2f &pos);
    // std::unique_ptr<DrawableEntity> clone() const override;
    ~AnimatedEntity() override = default;
    void animate();
    void update() override;
    void draw(sf::RenderWindow& window) const override;
    sf::Vector2f getPosition() const override;
    void setPosition(const sf::Vector2f& pos) override;
    sf::FloatRect getGlobalBounds() const override;
    friend std::ostream & operator<<(std::ostream &os, const AnimatedEntity &obj);

};