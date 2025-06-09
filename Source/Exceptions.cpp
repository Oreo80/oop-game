#include "../Headers/Exceptions.h"

GameException::GameException(std::string msg): message(std::move(msg)) {}

const char * GameException::what() const noexcept {
    return message.c_str();
}

TextureLoadException::TextureLoadException(const std::string &path): GameException("Failed to load texture from: " + path) {}

SFMLFontLoadException::SFMLFontLoadException(const std::string &path): GameException("Failed to load sf::Font from: " + path) {}

MusicLoadException::MusicLoadException(const std::string &path): GameException("Failed to load music from: " + path) {}

SoundLoadException::SoundLoadException(const std::string &path): GameException("Failed to load sound buffer from: " + path) {}

JSONLoadException::JSONLoadException(const std::string &path) : GameException("Failed to load JSON from: " + path) {}

InvalidSpriteStateException::InvalidSpriteStateException(const std::string &state): GameException("Invalid state: " + state) {}

GameLogicException::GameLogicException(const std::string &description): GameException("Game logic error: " + description) {}

FontMetadataLoadException::FontMetadataLoadException(const std::string &path):
GameException("Failed to open font metadata file: " + path) {}

FontMetadataParseException::FontMetadataParseException(const int line, const std::string &token):
GameException("Error parsing number at line " + std::to_string(line)
    + ": '" + token + "' is not a valid integer") {}

FontMetadataFormatException::FontMetadataFormatException(const int line, const size_t got, const size_t expected):
GameException("Invalid field count at line " + std::to_string(line)
    + ": got " + std::to_string(got)
    + ", expected " + std::to_string(expected)) {}
