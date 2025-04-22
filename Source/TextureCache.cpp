#include "../Headers/TextureCache.h"

std::map<std::string, std::shared_ptr<sf::Texture>> TextureCache::textures;

std::shared_ptr<sf::Texture> TextureCache::get(const std::string& path) {
    if (const auto i = textures.find(path); i != textures.end())
        return i->second;

    auto tex = std::make_shared<sf::Texture>();
    if (!tex->loadFromFile(path)) throw std::runtime_error("Failed to load " + path);
    textures[path] = tex;
    return tex;
}
std::ostream& operator<<(std::ostream& os, const TextureCache& obj) {
    os << "=== Texture Cache ===\n";
    os << "Total cached textures: " << obj.textures.size() << "\n";

    for (const auto&[first, second] : obj.textures) {
        const sf::Texture& texture = *second;
        os << " - Path: '" << first << "'\n"
           << "   References: " << second.use_count() << "\n"
           << "   Size: " << texture.getSize().x << "x" << texture.getSize().y << "\n";
    }

    return os;
}