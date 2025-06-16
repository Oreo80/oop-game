#pragma once
#include "ResourceManager.h"
#include "SpriteEntity.h"

class AnimatedEntity : public SpriteEntity {
private:
    friend void swap(AnimatedEntity &lhs, AnimatedEntity &rhs) noexcept;
protected:
    std::vector<std::shared_ptr<sf::Texture>> frames;
    std::size_t currentFrame = 0;
    sf::Clock animationClock;
    int animationInterval;

    static std::vector<std::shared_ptr<sf::Texture>> loadTextures(const std::vector<std::string>& paths);
    void print(std::ostream& os) const override;

public:
    explicit AnimatedEntity(const std::vector<std::string> &paths, const sf::Vector2f &pos, int interval = 100);
    AnimatedEntity(const AnimatedEntity &other);
    AnimatedEntity(AnimatedEntity &&other) noexcept;
    AnimatedEntity & operator=(AnimatedEntity other);
    std::unique_ptr<DrawableEntity> clone() const override;
    ~AnimatedEntity() override = default;
    void animate();
    void update() override;
};