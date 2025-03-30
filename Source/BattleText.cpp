#include "../Headers/BattleText.h"


std::ostream & operator<<(std::ostream &os, const BattleText &obj){
    return os
            << "Full text: "<< obj.fullText
            <<" , Current length: "<<obj.currentLength
            << " , Text position ("<<obj.text.getPosition().x<<", "<<obj.text.getPosition().y<<") "
            << "Font size: "<<obj.fontSize;
}

void BattleText::setText(const std::string& newText, const float delayTime) {
    fullText = newText;
    currentLength = 0;
    text.setString("");
    textStartDelay = delayTime;
    textStarted = false;
    textClock.restart();
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
    }
}

void BattleText::draw(sf::RenderWindow& window) const{
    if (textStarted) {
        window.draw(text);
    }
}
