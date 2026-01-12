#include "LoginScreen.h"
#include <iostream>
#include "FeedScreen.h"

LoginScreen::LoginScreen(App& app)
    : app(app),
    font(),
    title(font),
    userText(font),
    passText(font),
    loginText(font),
    registerText(font),
    statusText(font),
    userActive(false),
    passActive(false)
{
    if (!font.openFromFile("font/Inter_28pt-Regular.ttf"))
    {
        throw std::runtime_error("Font load failed");
    }

    title.setString("Social Media System");
    title.setCharacterSize(36);
    title.setFillColor(sf::Color::White);
    title.setPosition({ 200.f, 40.f });

    userBox.setSize({ 300.f, 40.f });
    userBox.setPosition({ 200.f, 120.f });
    userBox.setFillColor(sf::Color(60, 60, 60));
    userBox.setOutlineColor(sf::Color::Cyan);
    userBox.setOutlineThickness(2.f);

    passBox.setSize({ 300.f, 40.f });
    passBox.setPosition({ 200.f, 180.f });
    passBox.setFillColor(sf::Color(60, 60, 60));
    passBox.setOutlineColor(sf::Color::Cyan);
    passBox.setOutlineThickness(2.f);

    loginBtn.setSize({ 140.f, 40.f });
    loginBtn.setPosition({ 200.f, 260.f });
    loginBtn.setFillColor(sf::Color(100, 200, 100));

    registerBtn.setSize({ 140.f, 40.f });
    registerBtn.setPosition({ 360.f, 260.f });
    registerBtn.setFillColor(sf::Color(100, 100, 200));

    userText.setCharacterSize(20);
    userText.setFillColor(sf::Color::White);
    userText.setPosition({ 210.f, 125.f });

    passText.setCharacterSize(20);
    passText.setFillColor(sf::Color::White);
    passText.setPosition({ 210.f, 185.f });

    loginText.setString("Login");
    loginText.setCharacterSize(20);
    loginText.setFillColor(sf::Color::White);
    loginText.setPosition({ 240.f, 265.f });

    registerText.setString("Register");
    registerText.setCharacterSize(20);
    registerText.setFillColor(sf::Color::White);
    registerText.setPosition({ 380.f, 265.f });

    statusText.setCharacterSize(18);
    statusText.setFillColor(sf::Color::Yellow);
    statusText.setPosition({ 200.f, 320.f });
}

void LoginScreen::handleEvent(const sf::Event& event, sf::RenderWindow&)
{
    if (const auto* mouse = event.getIf<sf::Event::MouseButtonPressed>())
    {
        sf::Vector2f pos{
            static_cast<float>(mouse->position.x),
            static_cast<float>(mouse->position.y)
        };

        userActive = userBox.getGlobalBounds().contains(pos);
        passActive = passBox.getGlobalBounds().contains(pos);

        // LOGIN BUTTON
        if (loginBtn.getGlobalBounds().contains(pos))
        {
            std::cout << "Login button clicked" << std::endl;
            std::cout << "Username: " << username << ", Password: " << password << std::endl;

            if (app.login(username, password))
            {
                std::cout << "Login successful, switching to FeedScreen" << std::endl;
                statusText.setString("Login successful");
                app.setScreen(new FeedScreen(app));
            }
            else
            {
                std::cout << "Login failed" << std::endl;
                statusText.setString("Invalid credentials");
            }
        }

        // REGISTER BUTTON
        if (registerBtn.getGlobalBounds().contains(pos))
        {
            std::cout << "Register button clicked" << std::endl;

            if (app.registerUser(username, password))
            {
                std::cout << "Registration successful, switching to FeedScreen" << std::endl;
                statusText.setString("Registered successfully");
                app.setScreen(new FeedScreen(app));
            }
            else
            {
                std::cout << "Registration failed" << std::endl;
                statusText.setString("Registration failed - user may already exist");
            }
        }
    }

    if (const auto* text = event.getIf<sf::Event::TextEntered>())
    {
        if (text->unicode < 128 && text->unicode != 13) // Exclude enter key
        {
            char c = static_cast<char>(text->unicode);

            if (c == '\b') // Backspace
            {
                if (userActive && !username.empty()) username.pop_back();
                if (passActive && !password.empty()) password.pop_back();
            }
            else if (c >= 32) // Printable characters only
            {
                if (userActive) username += c;
                if (passActive) password += c;
            }
        }
    }
}

void LoginScreen::update(sf::Time)
{
    userText.setString(username.empty() ? "Username..." : username);
    passText.setString(password.empty() ? "Password..." : std::string(password.size(), '*'));

    // Update box colors based on active state
    userBox.setOutlineColor(userActive ? sf::Color::Green : sf::Color::Cyan);
    passBox.setOutlineColor(passActive ? sf::Color::Green : sf::Color::Cyan);
}

void LoginScreen::render(sf::RenderWindow& window)
{
    window.draw(title);
    window.draw(userBox);
    window.draw(passBox);
    window.draw(loginBtn);
    window.draw(registerBtn);
    window.draw(userText);
    window.draw(passText);
    window.draw(loginText);
    window.draw(registerText);
    window.draw(statusText);
}