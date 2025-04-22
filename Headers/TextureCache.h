#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <memory>

class TextureCache {
private:
    static std::map<std::string, std::shared_ptr<sf::Texture>> textures;
public:
    static std::shared_ptr<sf::Texture> get(const std::string& path);
    friend std::ostream& operator<<(std::ostream& os, const TextureCache& obj);
};