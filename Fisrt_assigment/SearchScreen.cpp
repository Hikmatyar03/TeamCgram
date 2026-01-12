#include "SearchScreen.h"
#include <variant> 
#include <SFML/Config.hpp>
#pragma message("SFML VERSION: " SFML_VERSION_STRING)

SearchScreen::SearchScreen(App& app)
    : app(app),
    inputText(font)   // ✅ REQUIRED in SFML 3.0
{
    if (!font.openFromFile("font/Inter_28pt-Regular.ttf"))
    {
        throw std::runtime_error("Failed to load font: Inter_28pt-Regular.ttf");
    }


    inputBox.setSize({ 300.f, 40.f });
    inputBox.setPosition({ 20.f, 20.f });
    inputBox.setFillColor(sf::Color::Cyan);

    inputText.setPosition({ 25.f, 25.f });
    inputText.setCharacterSize(18);
    inputText.setFillColor(sf::Color::Black);
}

void SearchScreen::handleEvent(const sf::Event& event, sf::RenderWindow&)
{
    if (const auto* textEntered = event.getIf<sf::Event::TextEntered>())
    {
        char32_t unicode = textEntered->unicode;

        if (unicode < 128)
        {
            char c = static_cast<char>(unicode);

            if (c == '\b')
            {
                if (!query.empty())
                    query.pop_back();
            }
            else
            {
                query += c;
            }
        }
    }
}
void SearchScreen::update(sf::Time)
{
    inputText.setString(query);
}


void SearchScreen::render(sf::RenderWindow& window) {

    window.draw(inputBox);
    window.draw(inputText);

    float y = 80.f;

    for (User* u : app.getUsers().searchUserByUsername(query)) {

        sf::Text name(font);   // ✅ REQUIRED
        name.setString(u->username);
        name.setCharacterSize(16);
        name.setFillColor(sf::Color::White);
        name.setPosition({ 20.f, y });

        window.draw(name);
        y += 30.f;
    }
}
