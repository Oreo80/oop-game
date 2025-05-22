#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <stack>
#include "Singleton.h"
#include "GameState.h"
#include "ResourceManager.h"
class GameState;
class GameManager : public Singleton<GameManager> {
private:
    friend class Singleton;
    std::shared_ptr<sf::Music> currentMusic;
    std::vector<sf::Sound> activeSounds;
    std::shared_ptr<sf::RenderWindow> window;
    bool isFullscreen = false;
    std::stack<std::unique_ptr<GameState>> states;
    int shakeFramesRemaining = 0;
    float shakeMagnitude = 2.f;
    sf::RectangleShape fadeOverlay;
    bool fading = false;
    float fadeDuration = 1.f;
    float fadeTimer = 0.f;
    bool fadeInActive = false;
    bool fadeOutActive = false;
    ResourceManager<sf::Texture> textures;
    ResourceManager<sf::Font> fonts;
    ResourceManager<sf::SoundBuffer> sounds;
    ResourceManager<sf::Music> music;



    GameManager() : window(std::make_shared<sf::RenderWindow>(sf::VideoMode({640, 480}), "Game", sf::Style::Titlebar | sf::Style::Close)) {
        window->setFramerateLimit(30);
        fadeOverlay.setSize(sf::Vector2f(640.f, 480.f));
        fadeOverlay.setFillColor(sf::Color(0, 0, 0, 0));
    }
    ~GameManager() = default;

    void handleEvents();
    void updateView() const;
    void updateFade(float dt);
    void drawFade() const;

public:

    GameManager(const GameManager&) = delete;
    GameManager& operator=(const GameManager&) = delete;
    void toggleFullscreen();
    std::shared_ptr<sf::RenderWindow> getWindow() { return window; }
    void triggerCameraShake(int frames = 2);
    void pushState(std::unique_ptr<GameState> state);

    void updateScreenShake();

    // void popState();
    // void clearStates();
    void playMusic(const std::string &path);
    void stopMusic() const;
    void playSound(const std::string& path);
    void fadeIn(float duration = 1.f);
    void fadeOut(float duration = 1.f);

    void run();


    void cleanupSounds();
};