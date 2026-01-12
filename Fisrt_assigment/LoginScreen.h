#pragma once
#include <SFML/Graphics.hpp>
#include "Screen.h"
#include "App.h"
#include <string>
#include <memory>

class LoginScreen : public Screen {
private:
    App& app;
    sf::Font font;

    // Use unique_ptr for sf::Text members
    std::unique_ptr<sf::Text> title;
    std::unique_ptr<sf::Text> subtitle;
    std::unique_ptr<sf::Text> userPlaceholder;
    std::unique_ptr<sf::Text> passPlaceholder;
    std::unique_ptr<sf::Text> userText;
    std::unique_ptr<sf::Text> passText;
    std::unique_ptr<sf::Text> loginText;
    std::unique_ptr<sf::Text> registerText;
    std::unique_ptr<sf::Text> statusText;
    std::unique_ptr<sf::Text> footerText;

    // Input boxes
    sf::RectangleShape userBox;
    sf::RectangleShape passBox;

    // Buttons
    sf::RectangleShape loginBtn;
    sf::RectangleShape registerBtn;

    // Input state
    std::string username;
    std::string password;
    bool userActive;
    bool passActive;

    // Animation
    float time;

public:
    explicit LoginScreen(App& app);
    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
    void update(sf::Time dt) override;
    void render(sf::RenderWindow& window) override;
};