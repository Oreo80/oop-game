#pragma once
#include "DrawableEntity.h"
#include <string>

class BattleText : public DrawableEntity{
private:
    sf::Font font;
    sf::Text text;
    unsigned int fontSize;
    std::string fullText;
    std::size_t currentLength = 0;
    float letterSpeed = 0.05f;
    float textStartDelay = 0.5f;
    bool textStarted = false;
    sf::Clock textClock;
    bool isuUpdating = false;

    static sf::Font loadFont();
public:
    explicit BattleText(sf::Vector2f position, unsigned int fontSize_);
    friend std::ostream & operator<<(std::ostream &os, const BattleText &obj);
    std::unique_ptr<DrawableEntity> clone() const override;
    void setText(const std::string& newText, float delayTime = 0.5f);
    void update() override;
    void draw(sf::RenderWindow& window) const override;
    sf::Vector2f getScale() const;
    void setScale(sf::Vector2f newScale);
    sf::FloatRect getGlobalBounds() const override;
    void setPosition(const sf::Vector2f& pos) override;
    sf::Vector2f getPosition() const override;
    ~BattleText() override = default;

};