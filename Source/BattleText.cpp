#include "../Headers/BattleText.h"

void BattleText::print(std::ostream &os) const {
    BitmapFont::print(os);
    os << ", Full text: "<< fullText << ", Current length: "<< currentLength;
}

BattleText::BattleText(const std::string &texturePath, const std::string &metadataPath, const sf::Vector2f &position,
                       const float scale): BitmapFont(texturePath, metadataPath, "", position, sf::Color::White, scale) {}

std::unique_ptr<DrawableEntity> BattleText::clone() const {
    return std::make_unique<BattleText>(*this);
}

void BattleText::setText(const std::string& newText, const float delayTime) {
    fullText = newText;
    currentLength = 0;
    textStartDelay = delayTime;
    textStarted = false;
    textClock.restart();
    isUpdating = true;
}

void BattleText::update() {
    if (!isUpdating) return;
    if (!textStarted) {
        if (textClock.getElapsedTime().asSeconds() >= textStartDelay) {
            textStarted = true;
            textClock.restart();
        }
        return;
    }
    if (currentLength < fullText.size() &&
        textClock.getElapsedTime().asSeconds() >= letterSpeed) {
        currentLength++;
        textClock.restart();
    } else if (currentLength >= fullText.size()) {
        isUpdating = false;
    }
}

void BattleText::draw(sf::RenderWindow& window) const {
    if (!textStarted) return;
    const std::string substr = fullText.substr(0, currentLength);
    const_cast<BattleText*>(this)->BitmapFont::setText(substr);
    const_cast<BattleText*>(this)->drawText(window);
}

std::ostream & operator<<(std::ostream &os, const BattleText &obj){
    obj.print(os);
    return os;
}
