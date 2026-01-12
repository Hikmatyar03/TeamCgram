#pragma once
#include "Screen.h"
#include "App.h"

class NotificationScreen : public Screen {
private:
    App& app;
    sf::Font font;

public:
    explicit NotificationScreen(App& app);

    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
    void update(sf::Time delta) override;
    void render(sf::RenderWindow& window) override;
};
