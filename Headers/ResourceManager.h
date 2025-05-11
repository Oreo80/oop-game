#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include <stdexcept>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
template<typename Resource>
class ResourceManager {
private:
    std::unordered_map<std::string, std::shared_ptr<Resource>> items;

public:
    ResourceManager() = default;
    ~ResourceManager() = default;
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    std::shared_ptr<Resource> get(const std::string& path) {
        auto it = items.find(path);
        if (it != items.end()) {
            return it->second;
        }

        auto res = std::make_shared<Resource>();
        if constexpr (std::is_same_v<Resource, sf::Texture>) {
            if (!res->loadFromFile(path))
                throw std::runtime_error("Failed to load texture: " + path);
        }
        else if constexpr (std::is_same_v<Resource, sf::Font>) {
            if (!res->openFromFile(path))
                throw std::runtime_error("Failed to load font: " + path);
        }
        else if constexpr (std::is_same_v<Resource, sf::SoundBuffer>) {
            if (!res->loadFromFile(path))
                throw std::runtime_error("Failed to load sound buffer: " + path);
        }
        else if constexpr (std::is_same_v<Resource, sf::Music>) {
            if (!res->openFromFile(path))
                throw std::runtime_error("Failed to load music: " + path);
        }
        else {
            static_assert(
                std::is_same_v<Resource, void>,
                "ResourceManager: unsupported resource type. Please specialize or extend."
            );
        }

        items[path] = res;
        return res;
    }
    [[nodiscard]] size_t size() const {
        return items.size();
    }
    void clear() {
        items.clear();
    }
};
