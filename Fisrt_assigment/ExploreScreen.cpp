#include "ExploreScreen.h"
#include "FeedScreen.h"
#include "ProfileScreen.h"
#include <iostream>
#include <algorithm>

ExploreScreen::ExploreScreen(App& app)
    : app(app),
    scrollOffset(0.f),
    targetScroll(0.f),
    time(0.f),
    header(font),      // Initialize with font
    backText(font)     // Initialize with font
{
    if (!font.openFromFile("font/Inter_28pt-Regular.ttf")) {
        std::cerr << "Font load failed\n";
    }

    header.setString("Discover Users");
    header.setCharacterSize(32);
    header.setFillColor(sf::Color(100, 200, 255));
    header.setStyle(sf::Text::Bold);
    header.setPosition(sf::Vector2f(280.f, 15.f));

    backBtn.setSize(sf::Vector2f(100.f, 40.f));
    backBtn.setPosition(sf::Vector2f(20.f, 15.f));
    backBtn.setFillColor(sf::Color(60, 60, 80));
    backBtn.setOutlineColor(sf::Color(100, 150, 200));
    backBtn.setOutlineThickness(2.f);

    backText.setString("← Back");
    backText.setCharacterSize(18);
    backText.setFillColor(sf::Color::White);
    backText.setPosition(sf::Vector2f(30.f, 22.f));
}

void ExploreScreen::handleEvent(const sf::Event& event, sf::RenderWindow&) {
    if (!app.getCurrentUser()) return;

    if (const auto* wheel = event.getIf<sf::Event::MouseWheelScrolled>()) {
        targetScroll += wheel->delta * 100.f;
        targetScroll = std::clamp(targetScroll, -2000.f, 0.f);
    }

    if (const auto* mouse = event.getIf<sf::Event::MouseButtonPressed>()) {
        sf::Vector2f pos(static_cast<float>(mouse->position.x),
            static_cast<float>(mouse->position.y));

        if (backBtn.getGlobalBounds().contains(pos)) {
            app.setScreen(new FeedScreen(app));
            return;
        }

        // User cards interaction
        float y = 100.f + scrollOffset;
        auto allUsers = app.getUsers().getAllUsers();

        for (User* user : allUsers) {
            if (user->userID == app.getCurrentUser()->userID) continue;

            // Check if following
            bool isFollowing = std::find(
                app.getCurrentUser()->following.begin(),
                app.getCurrentUser()->following.end(),
                user->userID
            ) != app.getCurrentUser()->following.end();

            sf::FloatRect followBtn(
                sf::Vector2f(600.f, y + 60.f),
                sf::Vector2f(isFollowing ? 110.f : 90.f, 35.f)
            );
            sf::FloatRect profileBtn(
                sf::Vector2f(480.f, y + 60.f),
                sf::Vector2f(100.f, 35.f)
            );

            if (followBtn.contains(pos)) {
                if (isFollowing) {
                    // Unfollow
                    auto& following = app.getCurrentUser()->following;
                    following.erase(
                        std::remove(following.begin(), following.end(), user->userID),
                        following.end()
                    );
                }
                else {
                    // Follow
                    app.getCurrentUser()->follow(user->userID);
                }
                return;
            }

            if (profileBtn.contains(pos)) {
                app.setScreen(new ProfileScreen(app, user->userID));
                return;
            }

            y += 130.f;
        }
    }
}

void ExploreScreen::update(sf::Time dt) {
    if (!app.getCurrentUser()) return;
    scrollOffset += (targetScroll - scrollOffset) * 10.f * dt.asSeconds();
    time += dt.asSeconds();
}

void ExploreScreen::render(sf::RenderWindow& window) {
    if (!app.getCurrentUser()) return;

    // Background
    sf::RectangleShape bg(sf::Vector2f(800.f, 600.f));
    bg.setFillColor(sf::Color(20, 25, 35));
    window.draw(bg);

    // Header bar
    sf::RectangleShape headerBar(sf::Vector2f(800.f, 70.f));
    headerBar.setFillColor(sf::Color(30, 35, 45));
    window.draw(headerBar);

    window.draw(backBtn);
    window.draw(backText);
    window.draw(header);

    // Get all users
    auto allUsers = app.getUsers().getAllUsers();

    // Draw user cards
    float y = 100.f + scrollOffset;
    int userCount = 0;

    for (User* user : allUsers) {
        if (user->userID == app.getCurrentUser()->userID) continue;

        // Card with shadow
        sf::RectangleShape shadow(sf::Vector2f(730.f, 120.f));
        shadow.setPosition(sf::Vector2f(40.f, y + 5.f));
        shadow.setFillColor(sf::Color(0, 0, 0, 50));
        window.draw(shadow);

        sf::RectangleShape card(sf::Vector2f(720.f, 110.f));
        card.setPosition(sf::Vector2f(35.f, y));
        card.setFillColor(sf::Color(40, 45, 55));
        card.setOutlineColor(sf::Color(60, 70, 90));
        card.setOutlineThickness(2.f);
        window.draw(card);

        // Avatar placeholder
        sf::CircleShape avatar(30.f);
        avatar.setPosition(sf::Vector2f(50.f, y + 25.f));
        avatar.setFillColor(sf::Color(100, 150, 200));
        window.draw(avatar);

        sf::Text avatarText(font);
        avatarText.setString(std::string(1, std::toupper(user->username[0])));
        avatarText.setCharacterSize(32);
        avatarText.setFillColor(sf::Color::White);
        avatarText.setStyle(sf::Text::Bold);
        avatarText.setPosition(sf::Vector2f(65.f, y + 30.f));
        window.draw(avatarText);

        // Username
        sf::Text username(font);
        username.setString("@" + user->username);
        username.setCharacterSize(22);
        username.setFillColor(sf::Color(100, 200, 255));
        username.setStyle(sf::Text::Bold);
        username.setPosition(sf::Vector2f(130.f, y + 20.f));
        window.draw(username);

        // Stats
        int postCount = static_cast<int>(user->posts.getAllPosts().size());
        int followerCount = 0;
        for (User* u : allUsers) {
            if (std::find(u->following.begin(), u->following.end(), user->userID)
                != u->following.end()) {
                followerCount++;
            }
        }

        sf::Text stats(font);
        stats.setString(std::to_string(postCount) + " posts • " +
            std::to_string(followerCount) + " followers • " +
            std::to_string(user->following.size()) + " following");
        stats.setCharacterSize(14);
        stats.setFillColor(sf::Color(150, 150, 150));
        stats.setPosition(sf::Vector2f(130.f, y + 55.f));
        window.draw(stats);

        // View Profile button
        sf::RectangleShape profileBtn(sf::Vector2f(100.f, 35.f));
        profileBtn.setPosition(sf::Vector2f(480.f, y + 60.f));
        profileBtn.setFillColor(sf::Color(80, 120, 160));
        profileBtn.setOutlineColor(sf::Color(100, 150, 200));
        profileBtn.setOutlineThickness(2.f);
        window.draw(profileBtn);

        sf::Text profileText(font);
        profileText.setString("Profile");
        profileText.setCharacterSize(16);
        profileText.setFillColor(sf::Color::White);
        profileText.setPosition(sf::Vector2f(495.f, y + 68.f));
        window.draw(profileText);

        // Follow/Unfollow button
        bool isFollowing = std::find(
            app.getCurrentUser()->following.begin(),
            app.getCurrentUser()->following.end(),
            user->userID
        ) != app.getCurrentUser()->following.end();

        sf::RectangleShape followBtn(sf::Vector2f(isFollowing ? 110.f : 90.f, 35.f));
        followBtn.setPosition(sf::Vector2f(600.f, y + 60.f));
        followBtn.setFillColor(isFollowing ?
            sf::Color(60, 60, 80) : sf::Color(80, 160, 100));
        followBtn.setOutlineColor(isFollowing ?
            sf::Color(100, 100, 120) : sf::Color(100, 200, 120));
        followBtn.setOutlineThickness(2.f);

        // Button hover animation
        if (!isFollowing) {
            float pulse = 1.f + 0.05f * std::sin(time * 4.f + userCount);
            sf::Vector2f originalPos = followBtn.getPosition();
            sf::Vector2f originalSize = followBtn.getSize();
            followBtn.setOrigin(sf::Vector2f(originalSize.x / 2.f, originalSize.y / 2.f));
            followBtn.setPosition(sf::Vector2f(
                originalPos.x + originalSize.x / 2.f,
                originalPos.y + originalSize.y / 2.f
            ));
            followBtn.setScale(sf::Vector2f(pulse, pulse));
            window.draw(followBtn);
            followBtn.setScale(sf::Vector2f(1.f, 1.f));
            followBtn.setOrigin(sf::Vector2f(0.f, 0.f));
            followBtn.setPosition(originalPos);
        }
        else {
            window.draw(followBtn);
        }

        sf::Text followText(font);
        followText.setString(isFollowing ? "Unfollow" : "Follow");
        followText.setCharacterSize(16);
        followText.setFillColor(sf::Color::White);
        followText.setPosition(sf::Vector2f(isFollowing ? 610.f : 620.f, y + 68.f));
        window.draw(followText);

        y += 130.f;
        userCount++;
    }

    if (userCount == 0) {
        sf::Text emptyText(font);
        emptyText.setString("No other users yet.\nInvite your friends!");
        emptyText.setCharacterSize(20);
        emptyText.setFillColor(sf::Color(100, 100, 100));
        emptyText.setPosition(sf::Vector2f(280.f, 300.f));
        window.draw(emptyText);
    }
}