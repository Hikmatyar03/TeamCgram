#pragma once
#include <SFML/Graphics.hpp>

class App;

class Screen {
public:
    virtual ~Screen() = default;

    virtual void handleEvent(const sf::Event& event, sf::RenderWindow& window) = 0;
    virtual void update(sf::Time delta) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
};
#pragma once
