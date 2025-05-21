#pragma once
#include <stdexcept>
#include <string>


class GameException : public std::exception {
protected:
    std::string message;
public:
    explicit GameException(std::string  msg);
    [[nodiscard]] const char* what() const noexcept override;
    ~GameException() override = default;
};

class TextureLoadException : public GameException {
public:
   explicit TextureLoadException(const std::string& path);
};
class SFMLFontLoadException : public GameException {
public:
    explicit SFMLFontLoadException(const std::string& path);
};
class MusicLoadException : public GameException {
public:
    explicit MusicLoadException(const std::string& path);
};
class SoundLoadException : public GameException {
public:
    explicit SoundLoadException(const std::string& path);
};

class InvalidSpriteStateException : public GameException {
public:
    explicit InvalidSpriteStateException(const std::string& state);
};

class GameLogicException : public GameException {
public:
    explicit GameLogicException(const std::string& description);
};

class FontMetadataLoadException : public GameException {
public:
    explicit FontMetadataLoadException(const std::string& path);
};

class FontMetadataParseException : public GameException {
public:
    FontMetadataParseException(int line, const std::string& token);
};

class FontMetadataFormatException : public GameException {
public:
    FontMetadataFormatException(int line, size_t got, size_t expected);
};