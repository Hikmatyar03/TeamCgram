#include "NotificationScreen.h"
#include "App.h"  // Include full definition here
#include "FeedScreen.h"
#include "NotificationSystem.h"  // Include for Notification types
#include <iostream>
#include <algorithm>
#include <ctime>

NotificationScreen::NotificationScreen(App& app)
    : app(app), scrollOffset(0.f), targetScroll(0.f), time(0.f) {

    if (!font.openFromFile("font/Inter_28pt-Regular.ttf")) {
        std::cerr << "Font load failed\n";
    }

    // Initialize with make_unique
    header = std::make_unique<sf::Text>(font);
    header->setString("Notifications");
    header->setCharacterSize(32);
    header->setFillColor(sf::Color(100, 200, 255));
    header->setStyle(sf::Text::Bold);
    header->setPosition({ 250.f, 15.f });

    backBtn.setSize({ 100.f, 40.f });
    backBtn.setPosition({ 20.f, 15.f });
    backBtn.setFillColor(sf::Color(60, 60, 80));
    backBtn.setOutlineColor(sf::Color(100, 150, 200));
    backBtn.setOutlineThickness(2.f);

    backText = std::make_unique<sf::Text>(font);
    backText->setString("← Back");
    backText->setCharacterSize(18);
    backText->setFillColor(sf::Color::White);
    backText->setPosition({ 30.f, 22.f });

    clearAllBtn.setSize({ 120.f, 40.f });
    clearAllBtn.setPosition({ 660.f, 15.f });
    clearAllBtn.setFillColor(sf::Color(80, 60, 60));
    clearAllBtn.setOutlineColor(sf::Color(200, 100, 100));
    clearAllBtn.setOutlineThickness(2.f);

    clearAllText = std::make_unique<sf::Text>(font);
    clearAllText->setString("Clear All");
    clearAllText->setCharacterSize(16);
    clearAllText->setFillColor(sf::Color::White);
    clearAllText->setPosition({ 675.f, 24.f });
}

void NotificationScreen::handleEvent(const sf::Event& event, sf::RenderWindow&) {
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

        if (clearAllBtn.getGlobalBounds().contains(pos)) {
            app.getNotificationManager().markAllAsRead(app.getCurrentUser()->userID);
            return;
        }

        // Click on notification to mark as read
        float y = 100.f + scrollOffset;
        auto notifications = app.getNotificationManager()
            .getAllNotifications(app.getCurrentUser()->userID);

        for (Notification* notif : notifications) {
            sf::FloatRect notifRect({ 20.f, y }, { 760.f, 80.f });
            if (notifRect.contains(pos) && !notif->isRead) {
                app.getNotificationManager().markAsRead(notif->notificationID);
                return;
            }
            y += 90.f;
        }
    }
}

void NotificationScreen::update(sf::Time dt) {
    if (!app.getCurrentUser()) return;
    scrollOffset += (targetScroll - scrollOffset) * 10.f * dt.asSeconds();
    time += dt.asSeconds();
}

void NotificationScreen::render(sf::RenderWindow& window) {
    if (!app.getCurrentUser()) return;

    // Background
    sf::RectangleShape bg({ 800.f, 600.f });
    bg.setFillColor(sf::Color(20, 25, 35));
    window.draw(bg);

    // Header bar
    sf::RectangleShape headerBar({ 800.f, 70.f });
    headerBar.setFillColor(sf::Color(30, 35, 45));
    window.draw(headerBar);

    window.draw(backBtn);
    window.draw(*backText);
    window.draw(*header);
    window.draw(clearAllBtn);
    window.draw(*clearAllText);

    // Get notifications
    auto notifications = app.getNotificationManager()
        .getAllNotifications(app.getCurrentUser()->userID);

    if (notifications.empty()) {
        sf::Text emptyText(font);
        emptyText.setString("No notifications yet");
        emptyText.setCharacterSize(20);
        emptyText.setFillColor(sf::Color(100, 100, 100));
        emptyText.setPosition({ 300.f, 300.f });
        window.draw(emptyText);
        return;
    }

    // Draw notifications
    float y = 100.f + scrollOffset;
    for (Notification* notif : notifications) {
        // Card
        sf::RectangleShape card({ 760.f, 80.f });
        card.setPosition({ 20.f, y });
        card.setFillColor(notif->isRead ?
            sf::Color(40, 45, 55) : sf::Color(50, 60, 80));
        card.setOutlineColor(notif->isRead ?
            sf::Color(60, 70, 90) : sf::Color(100, 150, 200));
        card.setOutlineThickness(2.f);
        window.draw(card);

        // Icon based on type
        sf::Text icon(font);
        icon.setCharacterSize(32);
        icon.setPosition({ 30.f, y + 20.f });

        switch (notif->type) {
        case NotificationType::FOLLOW:
            icon.setString("👤");
            icon.setFillColor(sf::Color(100, 200, 100));
            break;
        case NotificationType::LIKE:
            icon.setString("❤");
            icon.setFillColor(sf::Color(255, 100, 120));
            break;
        case NotificationType::COMMENT:
            icon.setString("💬");
            icon.setFillColor(sf::Color(100, 150, 255));
            break;
        case NotificationType::SYSTEM:
            icon.setString("🔔");
            icon.setFillColor(sf::Color(255, 200, 100));
            break;
        }
        window.draw(icon);

        // Message
        sf::Text message(font);
        message.setString(notif->message);
        message.setCharacterSize(16);
        message.setFillColor(sf::Color::White);
        message.setPosition({ 80.f, y + 20.f });
        window.draw(message);

        // Time ago
        time_t now = std::time(nullptr);
        int diff = static_cast<int>(now - notif->timestamp);
        std::string timeStr;
        if (diff < 60) timeStr = std::to_string(diff) + "s ago";
        else if (diff < 3600) timeStr = std::to_string(diff / 60) + "m ago";
        else if (diff < 86400) timeStr = std::to_string(diff / 3600) + "h ago";
        else timeStr = std::to_string(diff / 86400) + "d ago";

        sf::Text timeText(font);
        timeText.setString(timeStr);
        timeText.setCharacterSize(14);
        timeText.setFillColor(sf::Color(150, 150, 150));
        timeText.setPosition({ 80.f, y + 50.f });
        window.draw(timeText);

        // Unread indicator
        if (!notif->isRead) {
            sf::CircleShape dot(5.f);
            dot.setFillColor(sf::Color(100, 200, 255));
            dot.setPosition({ 750.f, y + 35.f });
            window.draw(dot);
        }

        y += 90.f;
    }
}