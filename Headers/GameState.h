#pragma once
#include <SFML/Graphics.hpp>
#include "GameManager.h"
#include "BitmapFont.h"
class GameManager;
class GameState {
private:
    void handleGlobalEvent(const std::optional<sf::Event> &event) const;

protected:
    GameManager& gameManager;
    std::shared_ptr<sf::RenderWindow> windowPtr;

    GameState();

    virtual void doProcessEvent(const std::optional<sf::Event> &event) = 0;
    virtual void doUpdate() = 0;
    virtual void doRender(sf::RenderWindow& window) const = 0;

    virtual void preUpdate() {}
    virtual void postUpdate() {}
    virtual void preRender(sf::RenderWindow&) const {}
    virtual void postRender(sf::RenderWindow&) const {}

    virtual void print(std::ostream& os) const { os<<"Game State: "; }

public:
    virtual ~GameState() = default;
    void processEvent(const std::optional<sf::Event> &event) {
        handleGlobalEvent(event);
        doProcessEvent(event);
    }

    void update() {
        preUpdate();
        doUpdate();
        postUpdate();
    }

    void render(sf::RenderWindow& window) const {
        preRender(window);
        doRender(window);
        postRender(window);
    }

    [[nodiscard]] virtual bool shouldChangeState() const { return false; }
    virtual std::unique_ptr<GameState> nextState() { return nullptr; }
    // [[nodiscard]] virtual std::unique_ptr<GameState> clone() const = 0;
    friend std::ostream& operator<<(std::ostream &os, const GameState &state);
};
