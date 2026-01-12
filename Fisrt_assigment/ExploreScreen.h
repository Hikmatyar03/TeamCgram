#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <cctype>
#include "Screen.h"
#include "App.h"

class ExploreScreen : public Screen {
private:
    App& app;

    // UI
    sf::Font font;
    sf::Text header;
    sf::RectangleShape backBtn;
    sf::Text backText;

    // Scrolling
    float scrollOffset;
    float targetScroll;

    // Animations
    std::map<int, float> buttonAnimations;
    float time;

public:
    explicit ExploreScreen(App& app);
    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
    void update(sf::Time dt) override;
    void render(sf::RenderWindow& window) override;
};