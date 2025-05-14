#pragma once
#include "DrawableEntity.h"
#include "BitmapFont.h"
#include "ResourceManager.h"

class BattleText : public DrawableEntity{
private:
    BitmapFont font;
    std::shared_ptr<sf::Texture> fontTexture;
    float scale;
    std::string fullText;
    std::size_t currentLength = 0;
    float letterSpeed = 0.05f;
    float textStartDelay = 0.5f;
    bool textStarted = false;
    sf::Clock textClock;
    sf::Vector2f position;
    bool isUpdating = false;
public:
    explicit BattleText(const std::string& texturePath, const std::string& metadataPath,
                        sf::Vector2f position, float scale = 0.8f);
    friend std::ostream & operator<<(std::ostream &os, const BattleText &obj);
    std::unique_ptr<DrawableEntity> clone() const override;
    void setText(const std::string& newText, float delayTime = 0.5f);
    void update() override;
    void draw(sf::RenderWindow& window) const override;
    sf::Vector2f getPosition() const override { return position; }
    void setPosition(const sf::Vector2f& pos) override { position = pos; }
    sf::FloatRect getGlobalBounds() const override;
    ~BattleText() override = default;

};