#pragma once
#include <SFML/Config.hpp>
#pragma message("SFML VERSION: " SFML_VERSION_STRING)

#include "Screen.h"
#include "App.h"

class SearchScreen : public Screen {
private:
    App& app;

    sf::Font font;
    sf::RectangleShape inputBox;
    sf::Text inputText;

    std::string query;

public:
    explicit SearchScreen(App& app);

    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
    void update(sf::Time delta) override;

    void render(sf::RenderWindow& window) override;
};
