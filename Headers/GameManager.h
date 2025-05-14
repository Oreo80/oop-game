#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <stack>
#include "Singleton.h"
#include "GameState.h"
#include "ResourceManager.h"
#include "BitmapFont.h"
class GameState;
class GameManager : public Singleton<GameManager> {
private:
    friend class Singleton;
    std::shared_ptr<sf::Music> currentMusic;
    std::shared_ptr<sf::RenderWindow> window;
    bool isFullscreen = false;

    std::stack<std::unique_ptr<GameState>> states;

    GameManager() : window(std::make_shared<sf::RenderWindow>(sf::VideoMode({640, 480}), "Game", sf::Style::Titlebar | sf::Style::Close)) {
        window->setFramerateLimit(30);
    }
    ~GameManager() = default;

    void handleEvents();
    void updateView() const;
public:
    ResourceManager<sf::Texture> textures;
    ResourceManager<sf::Font> fonts;
    ResourceManager<sf::SoundBuffer> sounds;
    ResourceManager<sf::Music> music;

    GameManager(const GameManager&) = delete;
    GameManager& operator=(const GameManager&) = delete;

    void toggleFullscreen();
    std::shared_ptr<sf::RenderWindow> getWindow() { return window; }

    void pushState(std::unique_ptr<GameState> state);
    void popState();
    // void clearStates();

    void run();

    void playMusic(const std::string &path);
};