#include "ProfileScreen.h"
#include "FeedScreen.h"
#include "LoginScreen.h"
#include <iostream>

ProfileScreen::ProfileScreen(App& app, int userID)
    : app(app),
    viewedUserID(userID),
    header(font),      // Initialize sf::Text with font
    followText(font)   // Initialize sf::Text with font
{
    if (!font.openFromFile("font/Inter_28pt-Regular.ttf")) {
        std::cerr << "Font load failed in ProfileScreen\n";
    }

    // Header text
    header.setCharacterSize(28);
    header.setFillColor(sf::Color::White);
    header.setStyle(sf::Text::Bold);
    header.setPosition(sf::Vector2f(20.f, 20.f));

    // Follow button
    followBtn.setSize(sf::Vector2f(120.f, 40.f));
    followBtn.setPosition(sf::Vector2f(650.f, 20.f));
    followBtn.setFillColor(sf::Color(80, 160, 240));
    followBtn.setOutlineColor(sf::Color(100, 180, 255));
    followBtn.setOutlineThickness(2.f);

    // Follow button text
    followText.setCharacterSize(18);
    followText.setFillColor(sf::Color::White);
    followText.setPosition(sf::Vector2f(680.f, 28.f));
}

void ProfileScreen::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    if (!app.getCurrentUser()) return;

    // Mouse clicks
    if (const auto* mouse = event.getIf<sf::Event::MouseButtonPressed>()) {
        sf::Vector2f pos(static_cast<float>(mouse->position.x),
            static_cast<float>(mouse->position.y));

        // Follow button
        if (followBtn.getGlobalBounds().contains(pos)) {
            User* currentUser = app.getCurrentUser();
            User* viewedUser = app.getUsers().getUserByID(viewedUserID);

            if (currentUser && viewedUser && currentUser->userID != viewedUserID) {
                currentUser->follow(viewedUserID);
            }
            return;
        }
    }

    // Back button (ESC key)
    if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
        if (key->scancode == sf::Keyboard::Scancode::Escape) {
            app.setScreen(new FeedScreen(app));
            return;
        }
    }
}

void ProfileScreen::update(sf::Time delta) {
    // Update logic if needed
}

void ProfileScreen::render(sf::RenderWindow& window) {
    if (!app.getCurrentUser()) return;

    User* viewedUser = app.getUsers().getUserByID(viewedUserID);
    if (!viewedUser) {
        sf::Text errorMsg(font);
        errorMsg.setString("User not found!");
        errorMsg.setCharacterSize(24);
        errorMsg.setFillColor(sf::Color::Red);
        errorMsg.setPosition({ 300.f, 250.f });
        window.draw(errorMsg);
        return;
    }

    // Background
    sf::RectangleShape bg(sf::Vector2f(800.f, 600.f));
    bg.setFillColor(sf::Color(20, 25, 35));
    window.draw(bg);

    // Header bar
    sf::RectangleShape headerBar(sf::Vector2f(800.f, 80.f));
    headerBar.setFillColor(sf::Color(30, 35, 45));
    window.draw(headerBar);

    // Display username
    header.setString("@" + viewedUser->username);
    window.draw(header);

    // Show follow button only if viewing someone else's profile
    if (app.getCurrentUser()->userID != viewedUserID) {
        bool isFollowing = std::find(
            app.getCurrentUser()->following.begin(),
            app.getCurrentUser()->following.end(),
            viewedUserID
        ) != app.getCurrentUser()->following.end();

        if (isFollowing) {
            followBtn.setFillColor(sf::Color(100, 100, 100));
            followText.setString("Following");
        }
        else {
            followBtn.setFillColor(sf::Color(80, 160, 240));
            followText.setString("Follow");
        }

        window.draw(followBtn);
        window.draw(followText);
    }

    // Display user stats
    sf::Text stats(font);
    stats.setCharacterSize(18);
    stats.setFillColor(sf::Color(200, 200, 200));
    stats.setPosition(sf::Vector2f(20.f, 100.f));

    int followerCount = 0;
    for (const auto& user : app.getUsers().getAllUsers()) {
        if (std::find(user->following.begin(), user->following.end(), viewedUserID)
            != user->following.end()) {
            followerCount++;
        }
    }

    stats.setString(
        "Followers: " + std::to_string(followerCount) + "\n" +
        "Following: " + std::to_string(viewedUser->following.size())
    );
    window.draw(stats);

    // Display user's posts
    sf::Text postsHeader(font);
    postsHeader.setString("Posts");
    postsHeader.setCharacterSize(24);
    postsHeader.setFillColor(sf::Color::White);
    postsHeader.setStyle(sf::Text::Bold);
    postsHeader.setPosition(sf::Vector2f(20.f, 180.f));
    window.draw(postsHeader);

    float yPos = 230.f;
    auto allPosts = app.getAllPosts();

    for (Post* post : allPosts) {
        if (post->userID == viewedUserID) {
            // Post card
            sf::RectangleShape card(sf::Vector2f(760.f, 80.f));
            card.setPosition(sf::Vector2f(20.f, yPos));
            card.setFillColor(sf::Color(40, 45, 55));
            card.setOutlineColor(sf::Color(60, 70, 90));
            card.setOutlineThickness(2.f);
            window.draw(card);

            // Post content
            sf::Text content(font);
            std::string displayContent = post->content;
            if (displayContent.length() > 100) {
                displayContent = displayContent.substr(0, 100) + "...";
            }
            content.setString(displayContent);
            content.setCharacterSize(16);
            content.setFillColor(sf::Color(220, 220, 220));
            content.setPosition(sf::Vector2f(30.f, yPos + 15.f));
            window.draw(content);

            // Like count
            sf::Text likeCount(font);
            likeCount.setString("❤ " + std::to_string(post->likes));
            likeCount.setCharacterSize(14);
            likeCount.setFillColor(sf::Color(255, 100, 120));
            likeCount.setPosition(sf::Vector2f(30.f, yPos + 50.f));
            window.draw(likeCount);

            yPos += 100.f;

            if (yPos > 550.f) break; // Don't draw off-screen
        }
    }

    // Back instruction
    sf::Text backHint(font);
    backHint.setString("Press ESC to go back");
    backHint.setCharacterSize(14);
    backHint.setFillColor(sf::Color(150, 150, 150));
    backHint.setPosition(sf::Vector2f(20.f, 570.f));
    window.draw(backHint);
}