#pragma once
#include "Screen.h"
#include "App.h"

class ProfileScreen : public Screen {
private:
    App& app;
    int viewedUserID;

    sf::Font font;

    // ❗ SFML 3: sf::Text MUST be constructed with a font
    sf::Text header;
    sf::Text followText;

    sf::RectangleShape followBtn;

public:
    ProfileScreen(App& app, int userID);

    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
    void update(sf::Time delta) override;
    void render(sf::RenderWindow& window) override;
};
