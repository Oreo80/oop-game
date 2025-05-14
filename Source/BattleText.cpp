#include "../Headers/BattleText.h"

BattleText::BattleText(const std::string& texturePath,
                       const std::string& metadataPath,
                       const sf::Vector2f position_,
                       const float scale_)
    : font(texturePath, metadataPath,"",position_,sf::Color::White,scale_), scale(scale_), position(position_){}

std::unique_ptr<DrawableEntity> BattleText::clone() const {
    return std::make_unique<BattleText>(*this);
}

void BattleText::setText(const std::string& newText, float delayTime) {
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
    const_cast<BitmapFont&>(font).setText(substr);
    const_cast<BitmapFont&>(font).drawText(window);
}

sf::FloatRect BattleText::getGlobalBounds() const {
    const float charWidth = 20.f * scale;
    const float lineHeight = 32.f * scale;
    float maxWidth = 0.f;
    float currentWidth = 0.f;
    int lines = 1;
    for (std::size_t i = 0; i < currentLength; ++i) {
        if (fullText[i] == '\n') {
            maxWidth = std::max(maxWidth, currentWidth);
            currentWidth = 0.f;
            lines++;
        } else {
            currentWidth += charWidth;
        }
    }
    maxWidth = std::max(maxWidth, currentWidth);
    float totalHeight = static_cast<float>(lines) * lineHeight;
    return { position, { maxWidth, totalHeight } };
}

std::ostream& operator<<(std::ostream& os, const BattleText& obj) {
    os << "Full text: " << obj.fullText
       << " , Current length: " << obj.currentLength
       << " , Position: (" << obj.position.x << ", " << obj.position.y << ")";
    return os;
}
