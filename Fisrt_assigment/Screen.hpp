#pragma once
#include <SFML/Graphics.hpp>

// ==================================================
// Base Screen Interface
// Every screen (Login, Feed, Profile, Notifications)
// MUST inherit from this class
// ==================================================
class Screen {
public:
    virtual ~Screen() = default;

    // Handle keyboard, mouse, input events
    virtual void handleEvent(const sf::Event& event) = 0;

    // Update logic (animations, timers, etc.)
    virtual void update() = 0;

    // Draw everything on window
    virtual void render(sf::RenderWindow& window) = 0;
};
