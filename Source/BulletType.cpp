#include "../Headers/BulletType.h"

BulletType::BulletType(const std::vector<std::string>& paths, const float spd)
    : speed(spd) {
    for (const auto& path : paths) {
        sf::Texture tex;
        if (!tex.loadFromFile(path)) {
            throw std::runtime_error("Failed to load texture: " + path);
        }
        textures.push_back(std::move(tex));
    }

    if (textures.empty()) {
        throw std::runtime_error("BulletType requires at least one texture");
    }
}

std::ostream & operator<<(std::ostream &os, const BulletType &obj) {
    return os
           << "Speed: " << obj.speed
           << ", Textures Loaded: " << obj.textures.size();
}

[[nodiscard]] const std::vector<sf::Texture>& BulletType::getTextures() const { return textures; }
[[nodiscard]] float BulletType::getSpeed() const { return speed; }

BulletType::BulletType(const BulletType &other)= default;

BulletType::BulletType(BulletType &&other) noexcept
    : textures(std::move(other.textures)),
      speed(other.speed){
}

BulletType & BulletType::operator=(const BulletType &other) {
    if (this == &other)
        return *this;
    textures = other.textures;
    speed = other.speed;
    return *this;
}

BulletType & BulletType::operator=(BulletType &&other) noexcept {
    if (this == &other)
        return *this;
    textures = std::move(other.textures);
    speed = other.speed;
    return *this;
}