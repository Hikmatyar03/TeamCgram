#include "LoginScreen.h"
#include "FeedScreen.h"
#include <iostream>
#include <cmath>

LoginScreen::LoginScreen(App& app)
    : app(app), userActive(false), passActive(false), time(0.f) {

    if (!font.openFromFile("font/Inter_28pt-Regular.ttf")) {
        throw std::runtime_error("Font load failed");
    }

    // App title with gradient effect - use make_unique
    title = std::make_unique<sf::Text>(font);
    title->setString("TeamCgram");
    title->setCharacterSize(48);
    title->setFillColor(sf::Color(100, 200, 255));
    title->setStyle(sf::Text::Bold);
    title->setPosition({ 260.f, 80.f });

    // Subtitle
    subtitle = std::make_unique<sf::Text>(font);
    subtitle->setString("Connect • Share • Inspire");
    subtitle->setCharacterSize(18);
    subtitle->setFillColor(sf::Color(150, 150, 150));
    subtitle->setPosition({ 290.f, 140.f });

    // Input boxes with modern styling
    userBox.setSize({ 400.f, 50.f });
    userBox.setPosition({ 200.f, 200.f });
    userBox.setFillColor(sf::Color(40, 45, 55));
    userBox.setOutlineColor(sf::Color(80, 120, 160));
    userBox.setOutlineThickness(2.f);

    passBox.setSize({ 400.f, 50.f });
    passBox.setPosition({ 200.f, 270.f });
    passBox.setFillColor(sf::Color(40, 45, 55));
    passBox.setOutlineColor(sf::Color(80, 120, 160));
    passBox.setOutlineThickness(2.f);

    // Placeholder text
    userPlaceholder = std::make_unique<sf::Text>(font);
    userPlaceholder->setString("Username");
    userPlaceholder->setCharacterSize(18);
    userPlaceholder->setFillColor(sf::Color(100, 100, 100));
    userPlaceholder->setPosition({ 215.f, 215.f });

    passPlaceholder = std::make_unique<sf::Text>(font);
    passPlaceholder->setString("Password");
    passPlaceholder->setCharacterSize(18);
    passPlaceholder->setFillColor(sf::Color(100, 100, 100));
    passPlaceholder->setPosition({ 215.f, 285.f });

    // Input text
    userText = std::make_unique<sf::Text>(font);
    userText->setCharacterSize(20);
    userText->setFillColor(sf::Color::White);
    userText->setPosition({ 215.f, 213.f });

    passText = std::make_unique<sf::Text>(font);
    passText->setCharacterSize(20);
    passText->setFillColor(sf::Color::White);
    passText->setPosition({ 215.f, 283.f });

    // Login button
    loginBtn.setSize({ 190.f, 50.f });
    loginBtn.setPosition({ 200.f, 360.f });
    loginBtn.setFillColor(sf::Color(80, 160, 240));
    loginBtn.setOutlineColor(sf::Color(100, 180, 255));
    loginBtn.setOutlineThickness(3.f);

    loginText = std::make_unique<sf::Text>(font);
    loginText->setString("Login");
    loginText->setCharacterSize(22);
    loginText->setFillColor(sf::Color::White);
    loginText->setStyle(sf::Text::Bold);
    loginText->setPosition({ 260.f, 372.f });

    // Register button
    registerBtn.setSize({ 190.f, 50.f });
    registerBtn.setPosition({ 410.f, 360.f });
    registerBtn.setFillColor(sf::Color(80, 160, 100));
    registerBtn.setOutlineColor(sf::Color(100, 200, 120));
    registerBtn.setOutlineThickness(3.f);

    registerText = std::make_unique<sf::Text>(font);
    registerText->setString("Register");
    registerText->setCharacterSize(22);
    registerText->setFillColor(sf::Color::White);
    registerText->setStyle(sf::Text::Bold);
    registerText->setPosition({ 450.f, 372.f });

    // Status message
    statusText = std::make_unique<sf::Text>(font);
    statusText->setCharacterSize(16);
    statusText->setFillColor(sf::Color::Yellow);
    statusText->setPosition({ 300.f, 440.f });

    // Footer
    footerText = std::make_unique<sf::Text>(font);
    footerText->setString("Develop By Hikmat Yar and Team");
    footerText->setCharacterSize(14);
    footerText->setFillColor(sf::Color(80, 80, 80));
    footerText->setPosition({ 310.f, 550.f });
}

void LoginScreen::handleEvent(const sf::Event& event, sf::RenderWindow&) {
    if (const auto* mouse = event.getIf<sf::Event::MouseButtonPressed>()) {
        sf::Vector2f pos(static_cast<float>(mouse->position.x),
            static_cast<float>(mouse->position.y));

        userActive = userBox.getGlobalBounds().contains(pos);
        passActive = passBox.getGlobalBounds().contains(pos);

        // Login button
        if (loginBtn.getGlobalBounds().contains(pos)) {
            if (username.empty() || password.empty()) {
                statusText->setString("Please fill in all fields");
                statusText->setFillColor(sf::Color(255, 100, 100));
                return;
            }

            std::cout << "Login attempt: " << username << std::endl;
            if (app.login(username, password)) {
                std::cout << "Login successful!" << std::endl;
                statusText->setString("Welcome back!");
                statusText->setFillColor(sf::Color(100, 255, 100));
                app.setScreen(new FeedScreen(app));
            }
            else {
                std::cout << "Login failed" << std::endl;
                statusText->setString("Invalid username or password");
                statusText->setFillColor(sf::Color(255, 100, 100));
            }
        }

        // Register button
        if (registerBtn.getGlobalBounds().contains(pos)) {
            if (username.empty() || password.empty()) {
                statusText->setString("Please fill in all fields");
                statusText->setFillColor(sf::Color(255, 100, 100));
                return;
            }

            if (username.length() < 3) {
                statusText->setString("Username must be at least 3 characters");
                statusText->setFillColor(sf::Color(255, 100, 100));
                return;
            }

            if (password.length() < 4) {
                statusText->setString("Password must be at least 4 characters");
                statusText->setFillColor(sf::Color(255, 100, 100));
                return;
            }

            std::cout << "Registration attempt: " << username << std::endl;
            if (app.registerUser(username, password)) {
                std::cout << "Registration successful!" << std::endl;
                statusText->setString("Account created! Welcome!");
                statusText->setFillColor(sf::Color(100, 255, 100));
                app.setScreen(new FeedScreen(app));
            }
            else {
                std::cout << "Registration failed" << std::endl;
                statusText->setString("Username already exists");
                statusText->setFillColor(sf::Color(255, 100, 100));
            }
        }
    }

    if (const auto* text = event.getIf<sf::Event::TextEntered>()) {
        if (text->unicode < 128 && text->unicode != 13) {
            char c = static_cast<char>(text->unicode);

            if (c == '\b') { // Backspace
                if (userActive && !username.empty()) username.pop_back();
                if (passActive && !password.empty()) password.pop_back();
            }
            else if (c >= 32 && c < 127) { // Printable characters
                if (userActive && username.length() < 20) username += c;
                if (passActive && password.length() < 30) password += c;
            }
        }
    }

    // Tab to switch fields
    if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
        if (key->scancode == sf::Keyboard::Scancode::Tab) {
            if (userActive) {
                userActive = false;
                passActive = true;
            }
            else if (passActive) {
                passActive = false;
                userActive = true;
            }
            else {
                userActive = true;
            }
        }
        else if (key->scancode == sf::Keyboard::Scancode::Enter) {
            // Auto-login on Enter
            if (!username.empty() && !password.empty()) {
                if (app.login(username, password)) {
                    app.setScreen(new FeedScreen(app));
                }
            }
        }
    }
}

void LoginScreen::update(sf::Time dt) {
    time += dt.asSeconds();

    // Update text displays using arrow operator
    userText->setString(username);
    passText->setString(std::string(password.size(), '•'));

    // Animate active field outline
    if (userActive) {
        float pulse = 150.f + 50.f * std::sin(time * 4.f);
        userBox.setOutlineColor(sf::Color(
            static_cast<std::uint8_t>(100 + pulse * 0.3f),
            static_cast<std::uint8_t>(180 + pulse * 0.3f),
            255
        ));
        userBox.setOutlineThickness(3.f);
    }
    else {
        userBox.setOutlineColor(sf::Color(80, 120, 160));
        userBox.setOutlineThickness(2.f);
    }

    if (passActive) {
        float pulse = 150.f + 50.f * std::sin(time * 4.f);
        passBox.setOutlineColor(sf::Color(
            static_cast<std::uint8_t>(100 + pulse * 0.3f),
            static_cast<std::uint8_t>(180 + pulse * 0.3f),
            255
        ));
        passBox.setOutlineThickness(3.f);
    }
    else {
        passBox.setOutlineColor(sf::Color(80, 120, 160));
        passBox.setOutlineThickness(2.f);
    }
}

void LoginScreen::render(sf::RenderWindow& window) {
    // Animated gradient background
    sf::RectangleShape bg1({ 800.f, 300.f });
    bg1.setPosition({ 0.f, 0.f });
    float wave1 = 20.f + 10.f * std::sin(time * 0.5f);
    bg1.setFillColor(sf::Color(
        static_cast<std::uint8_t>(20 + wave1),
        static_cast<std::uint8_t>(25 + wave1),
        static_cast<std::uint8_t>(35 + wave1)
    ));
    window.draw(bg1);

    sf::RectangleShape bg2({ 800.f, 300.f });
    bg2.setPosition({ 0.f, 300.f });
    float wave2 = 15.f + 8.f * std::sin(time * 0.7f);
    bg2.setFillColor(sf::Color(
        static_cast<std::uint8_t>(25 + wave2),
        static_cast<std::uint8_t>(30 + wave2),
        static_cast<std::uint8_t>(45 + wave2)
    ));
    window.draw(bg2);

    // Animated title - use arrow operator and dereference when drawing
    float titleBounce = std::sin(time * 2.f) * 3.f;
    title->setPosition({ 260.f, 80.f + titleBounce });
    window.draw(*title);        // Dereference
    window.draw(*subtitle);     // Dereference

    // Input boxes
    window.draw(userBox);
    window.draw(passBox);

    // Placeholders or text
    if (username.empty() && !userActive) {
        window.draw(*userPlaceholder);   // Dereference
    }
    else {
        window.draw(*userText);          // Dereference
    }

    if (password.empty() && !passActive) {
        window.draw(*passPlaceholder);   // Dereference
    }
    else {
        window.draw(*passText);          // Dereference
    }

    // Cursor blink for active field
    if ((userActive || passActive) && static_cast<int>(time * 2.f) % 2 == 0) {
        sf::RectangleShape cursor({ 2.f, 25.f });
        cursor.setFillColor(sf::Color::White);
        if (userActive) {
            float textWidth = userText->getLocalBounds().size.x;  // Arrow operator
            cursor.setPosition({ 215.f + textWidth + 5.f, 216.f });
        }
        else if (passActive) {
            float textWidth = passText->getLocalBounds().size.x;  // Arrow operator
            cursor.setPosition({ 215.f + textWidth + 5.f, 286.f });
        }
        window.draw(cursor);
    }

    // Buttons with hover animation
    float buttonPulse = 1.f + 0.05f * std::sin(time * 3.f);
    loginBtn.setScale({ buttonPulse, buttonPulse });
    window.draw(loginBtn);
    loginBtn.setScale({ 1.f, 1.f });
    window.draw(*loginText);     // Dereference

    float regPulse = 1.f + 0.05f * std::sin(time * 3.f + 1.5f);
    registerBtn.setScale({ regPulse, regPulse });
    window.draw(registerBtn);
    registerBtn.setScale({ 1.f, 1.f });
    window.draw(*registerText);  // Dereference

    // Status message
    window.draw(*statusText);    // Dereference

    // Footer
    window.draw(*footerText);    // Dereference
}