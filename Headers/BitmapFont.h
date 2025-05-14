#pragma once
#include <unordered_map>
#include <string>
#include <SFML/Graphics.hpp>

struct Glyph {
    sf::IntRect rect;
    int shift{};
    int offset{};
};

class BitmapFont {
private:
    std::unordered_map<char, Glyph> glyphs;
    std::shared_ptr<sf::Texture> texture;
    int lineHeight = 32;
    std::string text;
    sf::Vector2f position;
    sf::Color color;
    float scale;

    void load(const std::string& texturePath, const std::string& metadataPath);
    void loadTexture(const std::string& texturePath);
    void loadMetadata(const std::string &metadataPath);

    static std::vector<std::string> readMetadataLines(const std::string &metadataPath);

    Glyph parseGlyphLine(const std::string& line, int lineNumber);

public:
    explicit BitmapFont(const std::string& texturePath="./fonts/fnt_main.png",
        const std::string& metadataPath="./fonts/glyphs_fnt_main.csv", std::string text_ = "",
        sf::Vector2f position_ = {0,0}, sf::Color color_ = sf::Color::White, float scale = 0.8f);
    void drawText(sf::RenderTarget& window);
    void setText(const std::string& text_);

};
