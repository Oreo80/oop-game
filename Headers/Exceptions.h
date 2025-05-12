#pragma once
#include <stdexcept>
#include <string>


class GameException : public std::exception {
protected:
    std::string message;
public:
    explicit GameException(std::string  msg);
    ~GameException() override = default;
    [[nodiscard]] const char* what() const noexcept override;
};

class TextureLoadException : public GameException {
public:
   explicit TextureLoadException(const std::string& path);
};
class FontLoadException : public GameException {
public:
    explicit FontLoadException(const std::string& path);
};
class MusicLoadException : public GameException {
public:
    explicit MusicLoadException(const std::string& path);
};
class SoundLoadException : public GameException {
public:
    explicit SoundLoadException(const std::string& path);
};

class InvalidStateException : public GameException {
public:
    explicit InvalidStateException(const std::string& state);
};

class GameLogicException : public GameException {
public:
    explicit GameLogicException(const std::string& description);
};