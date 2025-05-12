#include "../Headers/Exceptions.h"

GameException::GameException(std::string msg): message(std::move(msg)) {}

const char * GameException::what() const noexcept { return message.c_str(); }

TextureLoadException::TextureLoadException(const std::string &path): GameException("Failed to load texture from: " + path) {}

FontLoadException::FontLoadException(const std::string &path): GameException("Failed to load font from: " + path) {}

MusicLoadException::MusicLoadException(const std::string &path): GameException("Failed to load music from: " + path) {}

SoundLoadException::SoundLoadException(const std::string &path): GameException("Failed to load sound buffer from: " + path) {}

InvalidStateException::InvalidStateException(const std::string &state): GameException("Invalid state: " + state) {}

GameLogicException::GameLogicException(const std::string &description): GameException("Game logic error: " + description) {}
