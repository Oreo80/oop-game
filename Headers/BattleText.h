#pragma once
#include "DrawableEntity.h"
#include "BitmapFont.h"
#include "ResourceManager.h"
class BattleText : public BitmapFont {
private:
    std::string fullText;
    std::size_t currentLength = 0;
    float letterSpeed;
    float textStartDelay;
    bool textStarted = false;
    sf::Clock textClock;
    bool isUpdating = false;
protected:
    void print(std::ostream& os) const override;
public:
    explicit BattleText(const std::string& texturePath="./fonts/fnt_main.png", const std::string& metadataPath="./fonts/glyphs_fnt_main.csv",
        const sf::Vector2f& position = {0,0}, float scale = 1.f, float letterSpeed_ = 0.05f, float textStartDelay_ = 0.5f);
    std::unique_ptr<DrawableEntity> clone() const override;
    void setText(const std::string& newText, float delayTime = 0.5f);
    void update() override;
    void draw(sf::RenderWindow& window) const override;
};