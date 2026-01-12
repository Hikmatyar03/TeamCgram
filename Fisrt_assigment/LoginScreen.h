#pragma once
#include "Screen.h"
#include "App.h"

class LoginScreen : public Screen {
private:
    App& app;

    sf::Font font;

    sf::Text title;
    sf::Text userText;
    sf::Text passText;
    sf::Text loginText;
    sf::Text registerText;
    sf::Text statusText;

    sf::RectangleShape userBox;
    sf::RectangleShape passBox;
    sf::RectangleShape loginBtn;
    sf::RectangleShape registerBtn;

    std::string username;
    std::string password;

    bool userActive;
    bool passActive;

public:
    explicit LoginScreen(App& app);

    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
    void update(sf::Time delta) override;
    void render(sf::RenderWindow& window) override;
};
