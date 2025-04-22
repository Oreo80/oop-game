#include "../Headers/BattleText.h"


std::ostream & operator<<(std::ostream &os, const BattleText &obj){
    return os
            << "Full text: "<< obj.fullText
            <<" , Current length: "<<obj.currentLength
            << " , Text position ("<<obj.text.getPosition().x<<", "<<obj.text.getPosition().y<<") "
            << "Font size: "<<obj.fontSize;
}

sf::Font BattleText::loadFont() {
    sf::Font font_;
    if (!font_.openFromFile("./fonts/DeterminationSans.ttf")) {
        std::cerr << "Failed to load font file!\n";
        throw std::runtime_error("Failed to load font file");
    }
    return font_;
}

void BattleText::setText(const std::string& newText, const float delayTime) {
    fullText = newText;
    currentLength = 0;
    text.setString("");
    textStartDelay = delayTime;
    textStarted = false;
    textClock.restart();
    isuUpdating=true;
}

void BattleText::update() {
    if (!textStarted) {
        if (textClock.getElapsedTime().asSeconds() >= textStartDelay) {
            textStarted = true;
            textClock.restart();
        }
        return;
    }

    if (currentLength < fullText.size() && textClock.getElapsedTime().asSeconds() >= letterSpeed) {
        currentLength++;
        text.setString(fullText.substr(0, currentLength));
        textClock.restart();
        isuUpdating=false;
    }
}

void BattleText::draw(sf::RenderWindow& window) const{
    if (textStarted) {
        window.draw(text);
    }
}

sf::Vector2f BattleText::getScale() const {
    return text.getScale();
}

void BattleText::setScale(const sf::Vector2f newScale) {
    text.setScale(newScale);
}
