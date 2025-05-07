#pragma once
#include "DrawableEntity.h"

class SpriteEntity : public DrawableEntity {
protected:
    sf::Texture texture;
    sf::Sprite sprite;
private:
    static sf::Texture initTexture(const std::string& texturePath);
public:
    explicit SpriteEntity(const std::string& texturePath="./img/missing.png", const sf::Vector2f& pos={0, 0});
    // std::unique_ptr<DrawableEntity> clone() const override;
    void draw(sf::RenderWindow& window) const override;
    sf::Vector2f getPosition() const override;
    void setPosition(const sf::Vector2f& pos) override;
    sf::FloatRect getGlobalBounds() const override;
    sf::Vector2f getOrigin() const;
    void setOrigin(const sf::Vector2f& origin_);
    ~SpriteEntity() override = default;

    friend std::ostream & operator<<(std::ostream &os, const SpriteEntity &obj);
};