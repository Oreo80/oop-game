#include "../Headers/GameManager.h"

#include <iostream>

#include "../Headers/SplashState.h"
#include "../Headers/PlayState.h"
#include <cstdint>
void GameManager::toggleFullscreen() {
    isFullscreen = !isFullscreen;

    if (isFullscreen) {
        window->create(sf::VideoMode::getDesktopMode(), "Game", sf::State::Fullscreen);
        window->setFramerateLimit(30);
    } else {
        window->create(sf::VideoMode({640, 480}), "Game", sf::State::Windowed);
        window->setFramerateLimit(30);
    }

    updateView();
}

void GameManager::triggerCameraShake(const int frames) {
    shakeFramesRemaining = frames;
}

void GameManager::pushState(std::unique_ptr<GameState> state) {
    states.push(std::move(state));
}

// void GameManager::popState() {
//     if(!states.empty()) {
//         states.pop();
//     }
//
//     if(states.empty()) {
//         window->close();
//     }
// }



// void GameManager::clearStates() {
//     while(!states.empty()) {
//         states.pop();
//     }
//     window->close();
// }

void GameManager::updateScreenShake() {
    if (shakeFramesRemaining > 0) {
        sf::View shake = window->getView();
        shake.move({-shakeMagnitude, -shakeMagnitude});
        window->setView(shake);
        --shakeFramesRemaining;
    }
}

void GameManager::run() {
    std::cout << "Running..." << std::endl;
    pushState(std::make_unique<SplashState>());

    sf::Clock clock;

    while(window->isOpen()) {
        const float dt = clock.restart().asSeconds();
        handleEvents();
        cleanupSounds();

        if(!states.empty()) {
            states.top()->update();

            if (states.top()->shouldChangeState()) {
                std::cout<<*(states.top())<<std::endl;
                auto newState = states.top()->nextState();
                states.pop();
                if (newState != nullptr) pushState(std::move(newState));
                if (states.empty()) {
                    window->close();
                    break;
                }
            }

            window->clear();
            updateView();
            updateScreenShake();
            updateFade(dt);
            states.top()->render(*window);
            drawFade();
            window->display();
        }
    }
}

void GameManager::playMusic(const std::string &path, const bool loop) {
    if(const auto newMusic = music.get(path); currentMusic != newMusic) {
        if(currentMusic) currentMusic->stop();
        currentMusic = newMusic;
        currentMusic->setLooping(loop);
        currentMusic->play();
    }
}

void GameManager::stopMusic() const {
    if (currentMusic) currentMusic->stop();
}

void GameManager::playSound(const std::string& path)
{
    const auto buffer = sounds.get(path);
    activeSounds.emplace_back(*buffer);
    sf::Sound& s = activeSounds.back();
    s.play();
}

void GameManager::cleanupSounds()
{
    std::erase_if(activeSounds, [](const sf::Sound &s) {
        return s.getStatus() == sf::Sound::Status::Stopped;
    });
}
void GameManager::fadeIn(const float duration) {
    fading = true;
    fadeInActive = true;
    fadeOutActive = false;
    fadeDuration = duration;
    fadeTimer = 0.f;
    fadeOverlay.setFillColor(sf::Color(0, 0, 0, 255));
}

void GameManager::fadeOut(const float duration) {
    fading = true;
    fadeOutActive = true;
    fadeInActive = false;
    fadeDuration = duration;
    fadeTimer = 0.f;
    fadeOverlay.setFillColor(sf::Color(0, 0, 0, 0));
}

void GameManager::handleEvents() {
    while (const std::optional<sf::Event> event = window->pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window->close();
        }
        if(!states.empty()) {
            states.top()->processEvent(event);
        }
    }
    }


void GameManager::updateView() const {
    const sf::Vector2u winSize = window->getSize();
    sf::Vector2f targetSize(640.f, 480.f);

    const float windowRatio = static_cast<float>(winSize.x) / static_cast<float>(winSize.y);
    const float targetRatio = targetSize.x / targetSize.y;

    sf::View view(sf::FloatRect({0, 0}, {targetSize.x, targetSize.y}));

    if (windowRatio > targetRatio) {
        const float newWidth = targetRatio * static_cast<float>(winSize.y);
        view.setViewport(sf::FloatRect({(1.f - newWidth / static_cast<float>(winSize.x)) / 2.f, 0.f},
                                       {newWidth / static_cast<float>(winSize.x), 1.f}));
    } else {
        const float newHeight = static_cast<float>(winSize.x) / targetRatio;
        view.setViewport(sf::FloatRect({0.f, (1.f - newHeight / static_cast<float>(winSize.y)) / 2.f},
                                       {1.f, newHeight / static_cast<float>(winSize.y)}));
    }

    window->setView(view);
}

void GameManager::updateFade(const float dt) {
    if (fadeInActive) {
        fadeTimer += dt;
        float alpha = 255.f * (1.f - (fadeTimer / fadeDuration));
        if (fadeTimer >= fadeDuration) {
            alpha = 0.f;
            fadeInActive = false;
            fading = false;
        }
        fadeOverlay.setFillColor(sf::Color(0, 0, 0, static_cast<std::uint8_t>(alpha)));
    } else if (fadeOutActive) {
        fadeTimer += dt;
        float alpha = 255.f * (fadeTimer / fadeDuration);
        if (fadeTimer >= fadeDuration) {
            alpha = 255.f;
            fadeOutActive = false;
            fading = false;
        }
        fadeOverlay.setFillColor(sf::Color(0, 0, 0, static_cast<std::uint8_t>(alpha)));
    }
}

void GameManager::drawFade() const {
    if (fading || fadeInActive || fadeOutActive) {
        window->draw(fadeOverlay);
    }
}
