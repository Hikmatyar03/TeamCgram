#pragma once
#include <SFML/Graphics.hpp>
#include "Screen.h"
#include <memory>

// Forward declaration to avoid circular dependency
class App;
class FeedScreen;

class NotificationScreen : public Screen {
private:
    App& app;
    sf::Font font;
    std::unique_ptr<sf::Text> header;
    sf::RectangleShape backBtn;
    std::unique_ptr<sf::Text> backText;
    sf::RectangleShape clearAllBtn;
    std::unique_ptr<sf::Text> clearAllText;
    float scrollOffset;
    float targetScroll;
    float time;

public:
    explicit NotificationScreen(App& app);
    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
    void update(sf::Time dt) override;
    void render(sf::RenderWindow& window) override;
};