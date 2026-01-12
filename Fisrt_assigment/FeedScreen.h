#pragma once
#include <SFML/Graphics.hpp>
#include "Screen.h"
#include "App.h"
#include <string>
#include <memory>

class FeedScreen : public Screen {
private:
    App& app;
    sf::Font font;

    // Header elements
    std::unique_ptr<sf::Text> title;
    sf::RectangleShape notificationBtn;
    std::unique_ptr<sf::Text> notificationText;
    sf::CircleShape notificationBadge;
    std::unique_ptr<sf::Text> notificationCount;
    sf::RectangleShape profileBtn;
    std::unique_ptr<sf::Text> profileText;

    // Post creation
    sf::RectangleShape postInputBox;
    std::unique_ptr<sf::Text> postInputText;
    std::string postContent;
    bool postInputActive;

    sf::RectangleShape createPostBtn;
    std::unique_ptr<sf::Text> createPostText;

    sf::RectangleShape logoutBtn;
    std::unique_ptr<sf::Text> logoutText;

    // Scrolling
    float scrollOffset;
    float targetScroll;
    float time;

    // Comment state
    int activeCommentPostID;
    std::string commentContent;

public:
    explicit FeedScreen(App& app);
    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
    void update(sf::Time dt) override;
    void render(sf::RenderWindow& window) override;

private:
    void renderPost(sf::RenderWindow& window, Post* post, float& yPos);
};