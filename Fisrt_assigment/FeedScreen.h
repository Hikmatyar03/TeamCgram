#pragma once

#include <SFML/Graphics.hpp>
#include "Screen.h"
#include "App.h"

class FeedScreen : public Screen
{
private:
    App& app;

    sf::Font font;
    sf::Text header;
    sf::Text logoutText;
    sf::RectangleShape logoutBtn;

    float scrollOffset;
    float targetScroll;

    // Add post
    sf::RectangleShape addPostBtn;
    sf::Text addPostText;
    bool addingPost;
    std::string postInput;
    sf::Text postInputText;

    // Comment
    bool addingComment;
    Post* activePost;
    std::string commentInput;
    sf::Text commentInputText;

public:
    explicit FeedScreen(App& app);

    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
    void update(sf::Time dt) override;
    void render(sf::RenderWindow& window) override;
};