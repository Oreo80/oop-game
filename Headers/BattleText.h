#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>

class BattleText {
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
    explicit BattleText(const sf::Vector2f position, const unsigned int fontSize_) :
    font(loadFont()),
    text(font,"",fontSize_),
    fontSize(fontSize_)
    {
        text.setFillColor(sf::Color::White);
        text.setPosition(position);
    }

    friend std::ostream & operator<<(std::ostream &os, const BattleText &obj);

    void setText(const std::string& newText, float delayTime = 0.5f);
    void update();
    void draw(sf::RenderWindow& window) const;
    sf::Vector2f getScale() const;
    void setScale(sf::Vector2f newScale);

};