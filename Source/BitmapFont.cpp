#include "../Headers/BitmapFont.h"
#include <fstream>
#include <sstream>
#include <utility>
#include "../Headers/Exceptions.h"
#include "../Headers/ResourceManager.h"

void BitmapFont::load(const std::string& texturePath,
                      const std::string& metadataPath)
{
    loadTexture(texturePath);
    loadMetadata(metadataPath);
    // practic, tot rostul la clasa asta e sa nu imi bat capul cu sf::Font
    // eu vreau sa pot sa fac jocul fullscreen, si indiferent de cum modificam sf::Font, aparea blurry
    // asa ca eu am decis sa fac o clasa care citeste dintr-o textura (spritesheet) mai multe caractere, numite glyphs
    // datele din csv, cat si textura in sine de font, sunt datamined din Undertale, dar daca as fi vrut as fi putut
    // sa le determin manual intr-un photo editor, orice, pana si paint
    // da, am incercat suficiente alternative inainte sa ajung la prostia asta, but it works
    // habar n-am de ce sf::Fontul nu se scaleaza bine, dar macar sprite-urile se scaleaza perfect
}

void BitmapFont::loadTexture(const std::string& texturePath) {
    ResourceManager<sf::Texture> textureManager;
    texture = textureManager.get(texturePath);
    if (!texture) {
        throw TextureLoadException(texturePath);
    }
}

void BitmapFont::loadMetadata(const std::string& metadataPath) {
    auto lines = readMetadataLines(metadataPath);
    int lineNumber = 0;
    for (const auto& line : lines) {
        ++lineNumber;
        // o linie din fisier trebuie sa fie sub formatul ASCII_code;posX;posY;width;height;shift;offset
        // unde posX si posY reprezinta coordonatele top-left ale glyph-ului (imaginea cu litera)
        // width si height reprezinta dimensiunea glyph-ului
        // shift se refera la numarul de pixeli cu care trebuie sa avanseze cursorul dupa ce a fost desenat caracterul
        // offset se refera la deplasarea orizontala la care trebuie sa desenezi caracterul in raport cu pozitia cursorului
        const Glyph glyph = parseGlyphLine(line, lineNumber);
        const int code = std::stoi(line.substr(0, line.find(';')));
        glyphs[static_cast<char>(code)] = glyph;
    }
}

std::vector<std::string> BitmapFont::readMetadataLines(const std::string& metadataPath)
{
    std::ifstream in(metadataPath);
    if (!in.is_open()) throw FontMetadataLoadException(metadataPath);

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(in, line)) {
        if (!line.empty()) lines.push_back(line);
    }
    return lines;
}


Glyph BitmapFont::parseGlyphLine(const std::string &line, const int lineNumber) {
    std::istringstream ss(line);
    std::string token;
    std::vector<int> values;
    while (std::getline(ss, token, ';')) {
        try {
            values.push_back(std::stoi(token));
        } catch (...) {
            throw FontMetadataParseException(lineNumber, token);
        }
    }
    if (values.size() != ExpectedGlyphFieldCount)
        throw FontMetadataFormatException(lineNumber, values.size(), ExpectedGlyphFieldCount);

    int posX = values[1], posY = values[2], width = values[3], height = values[4];
    if (height > lineHeight) lineHeight = height;

    return Glyph{ sf::IntRect({posX, posY}, {width, height}), values[5], values[6] };
}

void BitmapFont::print(std::ostream &os) const {
    os << "Text loaded: " << text << ", Position: ("<< position.x << ", " << position.y << "), Scale: " << scale;
}

BitmapFont::BitmapFont(const std::string &texturePath, const std::string &metadataPath, std::string text_,
                       const sf::Vector2f position_, const sf::Color color_, const float scale)
                     : text(std::move(text_)), position(position_), color(color_), scale(scale){
    load(texturePath, metadataPath);
}

void BitmapFont::drawText(sf::RenderTarget& window)
{
    sf::Sprite sprite(*texture);
    sprite.setColor(color);
    sf::Vector2f cursor = position;

    for (char c : text) {
        if (c == '\n') {
            cursor.x = position.x;
            cursor.y += static_cast<float>(lineHeight) * scale;
            continue;
        }

        auto it = glyphs.find(c);
        if (it == glyphs.end()) {
            cursor.x += 10 * scale;
            continue;
        }

        const Glyph& g = it->second;
        sprite.setTextureRect(g.rect);
        sprite.setPosition({cursor.x + static_cast<float>(g.offset) * scale, cursor.y});
        sprite.setScale({scale, scale});

        window.draw(sprite);

        cursor.x += static_cast<float>(g.shift) * scale;
    }
}

void BitmapFont::draw(sf::RenderWindow &window) const {
    const_cast<BitmapFont*>(this)->drawText(window);
}

void BitmapFont::setText(const std::string &text_) {
    text = text_;
}

void BitmapFont::setColor(const sf::Color color_) {
    color = color_;
}

void BitmapFont::setPosition(const sf::Vector2f &pos) {
    position=pos;
}

sf::Vector2f BitmapFont::getPositionForPlayer() const {
    return {position.x - 36, position.y + 8};
}

sf::Vector2f BitmapFont::getPosition() const {
    return position;
}
sf::FloatRect BitmapFont::getGlobalBounds() const {
    float width = 0.f;
    const float height = static_cast<float>(lineHeight) * scale;
    int lines = 1;

    for (const char c : text) {
        if (c == '\n') {
            lines++;
            width = std::max(width, 0.f);
        } else {
            width += 20.f * scale;
        }
    }
    return { position, { width, static_cast<float>(lines) * height } };
}


std::unique_ptr<DrawableEntity> BitmapFont::clone() const {
    return std::make_unique<BitmapFont>(*this);
}