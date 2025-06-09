#pragma once
#include <unordered_map>
#include <string>
#include <SFML/Graphics.hpp>
#include "DrawableEntity.h"

struct Glyph {
    sf::IntRect rect;
    int shift{};
    int offset{};
};

class BitmapFont : public DrawableEntity{
private:
    std::unordered_map<char, Glyph> glyphs;
    std::shared_ptr<sf::Texture> texture;
    int lineHeight = 32;
    std::string text;
    sf::Vector2f position;
    sf::Color color;
    float scale;
    static constexpr std::size_t ExpectedGlyphFieldCount = 7;

    void load(const std::string& texturePath, const std::string& metadataPath);
    void loadTexture(const std::string& texturePath);
    void loadMetadata(const std::string &metadataPath);
    static std::vector<std::string> readMetadataLines(const std::string &metadataPath);
    Glyph parseGlyphLine(const std::string& line, int lineNumber);
protected:
    void print(std::ostream& os) const override;
public:
    explicit BitmapFont(const std::string& texturePath="./fonts/fnt_main.png",
        const std::string& metadataPath="./fonts/glyphs_fnt_main.csv", std::string text_ = "",
        sf::Vector2f position_ = {0,0}, sf::Color color_ = sf::Color::White, float scale = 0.8f);
    void drawText(sf::RenderTarget& window);
    void draw(sf::RenderWindow &window) const override;
    void setText(const std::string& text_);
    void setColor (sf::Color color_);
    void setPosition(const sf::Vector2f& pos) override;
    sf::Vector2f getPositionForPlayer() const;
    sf::Vector2f getPosition() const override;
    sf::FloatRect getGlobalBounds() const override;
    std::unique_ptr<DrawableEntity> clone() const override;
};
