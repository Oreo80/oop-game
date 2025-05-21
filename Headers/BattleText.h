#pragma once
#include "DrawableEntity.h"
#include "BitmapFont.h"
#include "ResourceManager.h"
class BattleText : public BitmapFont {
private:
    std::string fullText;
    std::size_t currentLength = 0;
    float letterSpeed = 0.05f;
    float textStartDelay = 0.5f;
    bool textStarted = false;
    sf::Clock textClock;
    bool isUpdating = false;
protected:
    void print(std::ostream& os) const override;
public:
    explicit BattleText(const std::string& texturePath, const std::string& metadataPath, const sf::Vector2f& position, float scale = 1.f);
    std::unique_ptr<DrawableEntity> clone() const override;
    void setText(const std::string& newText, float delayTime = 0.5f);
    void update() override;
    void draw(sf::RenderWindow& window) const override;
};