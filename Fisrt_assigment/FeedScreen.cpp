#include "FeedScreen.h"
#include "NotificationScreen.h"
#include "ProfileScreen.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include "LoginScreen.h"


FeedScreen::FeedScreen(App& app)
    : app(app), postInputActive(false), scrollOffset(0.f),
    targetScroll(0.f), time(0.f), activeCommentPostID(-1) {

    if (!font.openFromFile("font/Inter_28pt-Regular.ttf")) {
        std::cerr << "Font load failed\n";
    }

    // Header - Modern gradient title
    title = std::make_unique<sf::Text>(font);
    title->setString("TeamCgram");
    title->setCharacterSize(36);
    title->setFillColor(sf::Color(100, 200, 255));
    title->setStyle(sf::Text::Bold);
    title->setPosition({ 30.f, 18.f });

    // Notification button
    notificationBtn.setSize({ 130.f, 45.f });
    notificationBtn.setPosition({ 530.f, 15.f });
    notificationBtn.setFillColor(sf::Color(50, 55, 70));
    notificationBtn.setOutlineColor(sf::Color(100, 150, 200));
    notificationBtn.setOutlineThickness(2.f);

    notificationText = std::make_unique<sf::Text>(font);
    notificationText->setString("Alerts");
    notificationText->setCharacterSize(16);
    notificationText->setFillColor(sf::Color::White);
    notificationText->setPosition({ 545.f, 26.f });

    // Notification badge
    notificationBadge.setRadius(10.f);
    notificationBadge.setFillColor(sf::Color(255, 60, 60));
    notificationBadge.setPosition({ 645.f, 15.f });

    notificationCount = std::make_unique<sf::Text>(font);
    notificationCount->setCharacterSize(12);
    notificationCount->setFillColor(sf::Color::White);
    notificationCount->setStyle(sf::Text::Bold);

    // Profile button
    profileBtn.setSize({ 100.f, 45.f });
    profileBtn.setPosition({ 680.f, 15.f });
    profileBtn.setFillColor(sf::Color(60, 120, 80));
    profileBtn.setOutlineColor(sf::Color(100, 200, 120));
    profileBtn.setOutlineThickness(2.f);

    profileText = std::make_unique<sf::Text>(font);
    profileText->setString("Profile");
    profileText->setCharacterSize(16);
    profileText->setFillColor(sf::Color::White);
    profileText->setPosition({ 695.f, 26.f });

    // Post input box - improved design
    postInputBox.setSize({ 740.f, 80.f });
    postInputBox.setPosition({ 30.f, 85.f });
    postInputBox.setFillColor(sf::Color(40, 45, 55));
    postInputBox.setOutlineColor(sf::Color(80, 120, 160));
    postInputBox.setOutlineThickness(2.f);

    postInputText = std::make_unique<sf::Text>(font);
    postInputText->setString("What's on your mind?");
    postInputText->setCharacterSize(18);
    postInputText->setFillColor(sf::Color(120, 120, 120));
    postInputText->setPosition({ 45.f, 110.f });

    // Create post button - more prominent
    createPostBtn.setSize({ 150.f, 45.f });
    createPostBtn.setPosition({ 620.f, 180.f });
    createPostBtn.setFillColor(sf::Color(80, 160, 240));
    createPostBtn.setOutlineColor(sf::Color(100, 180, 255));
    createPostBtn.setOutlineThickness(2.f);

    createPostText = std::make_unique<sf::Text>(font);
    createPostText->setString("Post");
    createPostText->setCharacterSize(18);
    createPostText->setFillColor(sf::Color::White);
    createPostText->setStyle(sf::Text::Bold);
    createPostText->setPosition({ 650.f, 190.f });

    // Logout button
    logoutBtn.setSize({ 100.f, 45.f });
    logoutBtn.setPosition({ 30.f, 540.f });
    logoutBtn.setFillColor(sf::Color(100, 50, 50));
    logoutBtn.setOutlineColor(sf::Color(200, 100, 100));
    logoutBtn.setOutlineThickness(2.f);

    logoutText = std::make_unique<sf::Text>(font);
    logoutText->setString("Logout");
    logoutText->setCharacterSize(16);
    logoutText->setFillColor(sf::Color::White);
    logoutText->setPosition({ 45.f, 551.f });
}

void FeedScreen::handleEvent(const sf::Event& event, sf::RenderWindow&) {
    if (!app.getCurrentUser()) return;

    // Mouse wheel scrolling
    if (const auto* wheel = event.getIf<sf::Event::MouseWheelScrolled>()) {
        targetScroll += wheel->delta * 100.f;
        targetScroll = std::clamp(targetScroll, -5000.f, 0.f);
    }

    if (const auto* mouse = event.getIf<sf::Event::MouseButtonPressed>()) {
        sf::Vector2f pos(static_cast<float>(mouse->position.x),
            static_cast<float>(mouse->position.y));

        // Notification button
        if (notificationBtn.getGlobalBounds().contains(pos)) {
            app.setScreen(new NotificationScreen(app));
            return;
        }

        // Profile button
        if (profileBtn.getGlobalBounds().contains(pos)) {
            app.setScreen(new ProfileScreen(app, app.getCurrentUser()->userID));
            return;
        }

        // Post input box
        if (postInputBox.getGlobalBounds().contains(pos)) {
            postInputActive = true;
            activeCommentPostID = -1;
        }
        else {
            postInputActive = false;
        }

        // Create post button
        if (createPostBtn.getGlobalBounds().contains(pos) && !postContent.empty()) {
            app.createPost(app.getCurrentUser()->userID, postContent);
            postContent.clear();
            postInputActive = false;
            targetScroll = 0.f;
            scrollOffset = 0.f;
        }

        // Logout button
        if (logoutBtn.getGlobalBounds().contains(pos)) {
            app.logout();
            app.setScreen(new LoginScreen(app));
            return;
        }

        // Handle post interactions
        float y = 245.f + scrollOffset;
        auto posts = app.getAllPosts();

        std::sort(posts.begin(), posts.end(), [](Post* a, Post* b) {
            return a->timestamp > b->timestamp;
            });

        for (Post* post : posts) {
            // Like button
            sf::FloatRect likeBtn({ 50.f, y + 100.f }, { 80.f, 35.f });
            if (likeBtn.contains(pos)) {
                auto& likes = post->likedBy;
                bool alreadyLiked = std::find(likes.begin(), likes.end(),
                    app.getCurrentUser()->userID) != likes.end();

                if (alreadyLiked) {
                    app.unlikePost(app.getCurrentUser()->userID, post->postID);
                }
                else {
                    app.likePost(app.getCurrentUser()->userID, post->postID);
                }
                return;
            }

            // Comment button
            sf::FloatRect commentBtn({ 150.f, y + 100.f }, { 100.f, 35.f });
            if (commentBtn.contains(pos)) {
                activeCommentPostID = post->postID;
                postInputActive = false;
                commentContent.clear();
                return;
            }

            // Comment input and submit
            if (activeCommentPostID == post->postID) {
                sf::FloatRect commentInput({ 50.f, y + 150.f }, { 600.f, 40.f });
                if (commentInput.contains(pos)) {
                    // Clicking comment input keeps it active
                    return;
                }

                sf::FloatRect submitBtn({ 670.f, y + 150.f }, { 100.f, 40.f });
                if (submitBtn.contains(pos) && !commentContent.empty()) {
                    app.addComment(post->postID, app.getCurrentUser()->userID, commentContent);
                    commentContent.clear();
                    activeCommentPostID = -1;
                    return;
                }
            }

            // Calculate next post position
            float postHeight = 145.f;
            if (activeCommentPostID == post->postID) {
                postHeight += 60.f;
            }

            // Add space for comments display
            Comment* c = post->comments;
            int commentCount = 0;
            while (c && commentCount < 3) {
                postHeight += 50.f;
                c = c->next;
                commentCount++;
            }

            y += postHeight + 15.f;
        }
    }

    // Text input
    if (const auto* text = event.getIf<sf::Event::TextEntered>()) {
        if (text->unicode < 128 && text->unicode != 13) {
            char c = static_cast<char>(text->unicode);

            if (c == '\b') {
                if (postInputActive && !postContent.empty()) {
                    postContent.pop_back();
                }
                else if (activeCommentPostID != -1 && !commentContent.empty()) {
                    commentContent.pop_back();
                }
            }
            else if (c >= 32 && c < 127) {
                if (postInputActive && postContent.length() < 280) {
                    postContent += c;
                }
                else if (activeCommentPostID != -1 && commentContent.length() < 200) {
                    commentContent += c;
                }
            }
        }
    }
}

void FeedScreen::update(sf::Time dt) {
    if (!app.getCurrentUser()) return;

    time += dt.asSeconds();
    scrollOffset += (targetScroll - scrollOffset) * 10.f * dt.asSeconds();

    // Update notification count
    int unreadCount = app.getNotificationManager().getUnreadCount(app.getCurrentUser()->userID);
    if (unreadCount > 0) {
        notificationCount->setString(std::to_string(unreadCount));
        sf::FloatRect bounds = notificationCount->getLocalBounds();
        notificationCount->setPosition({
            655.f - bounds.size.x / 2.f,
            22.f
            });
    }

    // Update post input text
    if (postInputActive) {
        postInputBox.setOutlineColor(sf::Color(120, 180, 255));
        postInputBox.setOutlineThickness(3.f);
    }
    else {
        postInputBox.setOutlineColor(sf::Color(80, 120, 160));
        postInputBox.setOutlineThickness(2.f);
    }
}

void FeedScreen::render(sf::RenderWindow& window) {
    if (!app.getCurrentUser()) return;

    // Background with subtle gradient
    sf::RectangleShape bg({ 800.f, 600.f });
    bg.setFillColor(sf::Color(25, 30, 40));
    window.draw(bg);

    // Header bar
    sf::RectangleShape headerBar({ 800.f, 75.f });
    headerBar.setFillColor(sf::Color(35, 40, 50));
    window.draw(headerBar);

    window.draw(*title);
    window.draw(notificationBtn);
    window.draw(*notificationText);

    // Draw notification badge if there are unread notifications
    int unreadCount = app.getNotificationManager().getUnreadCount(app.getCurrentUser()->userID);
    if (unreadCount > 0) {
        window.draw(notificationBadge);
        window.draw(*notificationCount);
    }

    window.draw(profileBtn);
    window.draw(*profileText);

    // Post input area
    window.draw(postInputBox);

    if (postContent.empty() && !postInputActive) {
        window.draw(*postInputText);
    }
    else if (!postContent.empty()) {
        sf::Text inputDisplay(font);
        inputDisplay.setString(postContent);
        inputDisplay.setCharacterSize(18);
        inputDisplay.setFillColor(sf::Color::White);
        inputDisplay.setPosition({ 45.f, 110.f });
        window.draw(inputDisplay);
    }

    // Cursor blink
    if (postInputActive && static_cast<int>(time * 2.f) % 2 == 0) {
        sf::RectangleShape cursor({ 2.f, 25.f });
        cursor.setFillColor(sf::Color::White);
        float textWidth = 0.f;
        if (!postContent.empty()) {
            sf::Text temp(font);
            temp.setString(postContent);
            temp.setCharacterSize(18);
            textWidth = temp.getLocalBounds().size.x;
        }
        cursor.setPosition({ 45.f + textWidth + 5.f, 112.f });
        window.draw(cursor);
    }

    window.draw(createPostBtn);
    window.draw(*createPostText);
    window.draw(logoutBtn);
    window.draw(*logoutText);

    // Feed section label
    sf::Text feedLabel(font);
    feedLabel.setString("Your Feed");
    feedLabel.setCharacterSize(24);
    feedLabel.setFillColor(sf::Color(150, 180, 220));
    feedLabel.setStyle(sf::Text::Bold);
    feedLabel.setPosition({ 30.f, 240.f });
    window.draw(feedLabel);

    // Render posts
    float y = 285.f + scrollOffset;
    auto posts = app.getAllPosts();

    std::sort(posts.begin(), posts.end(), [](Post* a, Post* b) {
        return a->timestamp > b->timestamp;
        });

    if (posts.empty()) {
        sf::Text emptyText(font);
        emptyText.setString("No posts yet. Be the first to post!");
        emptyText.setCharacterSize(20);
        emptyText.setFillColor(sf::Color(100, 100, 100));
        emptyText.setPosition({ 220.f, 350.f });
        window.draw(emptyText);
    }
    else {
        for (Post* post : posts) {
            renderPost(window, post, y);
        }
    }
}

void FeedScreen::renderPost(sf::RenderWindow& window, Post* post, float& yPos) {
    if (yPos > 600.f || yPos < -300.f) {
        yPos += 200.f;
        return;
    }

    // Post card with modern shadow effect
    sf::RectangleShape card({ 740.f, 135.f });
    card.setPosition({ 30.f, yPos });
    card.setFillColor(sf::Color(40, 45, 55));
    card.setOutlineColor(sf::Color(60, 70, 90));
    card.setOutlineThickness(2.f);
    window.draw(card);

    // Author
    User* author = app.getUsers().getUserByID(post->userID);
    std::string authorName = author ? author->username : "Unknown";

    sf::Text authorText(font);
    authorText.setString("@" + authorName);
    authorText.setCharacterSize(18);
    authorText.setFillColor(sf::Color(100, 200, 255));
    authorText.setStyle(sf::Text::Bold);
    authorText.setPosition({ 50.f, yPos + 15.f });
    window.draw(authorText);

    // Time ago
    time_t now = std::time(nullptr);
    int diff = static_cast<int>(now - post->timestamp);
    std::string timeStr;
    if (diff < 60) timeStr = std::to_string(diff) + "s";
    else if (diff < 3600) timeStr = std::to_string(diff / 60) + "m";
    else if (diff < 86400) timeStr = std::to_string(diff / 3600) + "h";
    else timeStr = std::to_string(diff / 86400) + "d";

    sf::Text timeText(font);
    timeText.setString(timeStr);
    timeText.setCharacterSize(14);
    timeText.setFillColor(sf::Color(120, 120, 120));
    timeText.setPosition({ 700.f, yPos + 20.f });
    window.draw(timeText);

    // Content
    sf::Text contentText(font);
    contentText.setString(post->content);
    contentText.setCharacterSize(16);
    contentText.setFillColor(sf::Color(220, 220, 220));
    contentText.setPosition({ 50.f, yPos + 50.f });
    window.draw(contentText);

    // Like button
    bool isLiked = std::find(post->likedBy.begin(), post->likedBy.end(),
        app.getCurrentUser()->userID) != post->likedBy.end();

    sf::RectangleShape likeBtn({ 80.f, 35.f });
    likeBtn.setPosition({ 50.f, yPos + 100.f });
    likeBtn.setFillColor(isLiked ? sf::Color(200, 80, 100) : sf::Color(60, 60, 80));
    likeBtn.setOutlineColor(sf::Color(100, 100, 120));
    likeBtn.setOutlineThickness(1.f);
    window.draw(likeBtn);

    sf::Text likeText(font);
    likeText.setString((isLiked ? "❤ " : "🤍 ") + std::to_string(post->likes));
    likeText.setCharacterSize(14);
    likeText.setFillColor(sf::Color::White);
    likeText.setPosition({ 58.f, yPos + 108.f });
    window.draw(likeText);

    // Comment button
    int commentCount = 0;
    Comment* c = post->comments;
    while (c) {
        commentCount++;
        c = c->next;
    }

    sf::RectangleShape commentBtn({ 100.f, 35.f });
    commentBtn.setPosition({ 150.f, yPos + 100.f });
    commentBtn.setFillColor(sf::Color(60, 60, 80));
    commentBtn.setOutlineColor(sf::Color(100, 100, 120));
    commentBtn.setOutlineThickness(1.f);
    window.draw(commentBtn);

    sf::Text commentBtnText(font);
    commentBtnText.setString("Comment" + std::to_string(commentCount));
    commentBtnText.setCharacterSize(14);
    commentBtnText.setFillColor(sf::Color::White);
    commentBtnText.setPosition({ 168.f, yPos + 108.f });
    window.draw(commentBtnText);

    yPos += 145.f;

    // Comment input if active
    if (activeCommentPostID == post->postID) {
        sf::RectangleShape commentInput({ 600.f, 40.f });
        commentInput.setPosition({ 50.f, yPos });
        commentInput.setFillColor(sf::Color(50, 55, 65));
        commentInput.setOutlineColor(sf::Color(100, 150, 200));
        commentInput.setOutlineThickness(2.f);
        window.draw(commentInput);

        sf::Text commentInputText(font);
        commentInputText.setString(commentContent.empty() ? "Write a comment..." : commentContent);
        commentInputText.setCharacterSize(14);
        commentInputText.setFillColor(commentContent.empty() ?
            sf::Color(120, 120, 120) : sf::Color::White);
        commentInputText.setPosition({ 60.f, yPos + 12.f });
        window.draw(commentInputText);

        // Submit button
        sf::RectangleShape submitBtn({ 100.f, 40.f });
        submitBtn.setPosition({ 670.f, yPos });
        submitBtn.setFillColor(sf::Color(80, 160, 240));
        submitBtn.setOutlineColor(sf::Color(100, 180, 255));
        submitBtn.setOutlineThickness(2.f);
        window.draw(submitBtn);

        sf::Text submitText(font);
        submitText.setString("Send");
        submitText.setCharacterSize(14);
        submitText.setFillColor(sf::Color::White);
        submitText.setPosition({ 695.f, yPos + 12.f });
        window.draw(submitText);

        yPos += 60.f;
    }

    // Display recent comments
    c = post->comments;
    int displayed = 0;
    while (c && displayed < 3) {
        User* commenter = app.getUsers().getUserByID(c->userID);
        std::string commenterName = commenter ? commenter->username : "Unknown";

        sf::RectangleShape commentBox({ 720.f, 45.f });
        commentBox.setPosition({ 50.f, yPos });
        commentBox.setFillColor(sf::Color(35, 40, 50));
        window.draw(commentBox);

        sf::Text commenterText(font);
        commenterText.setString("@" + commenterName + ":");
        commenterText.setCharacterSize(13);
        commenterText.setFillColor(sf::Color(100, 200, 255));
        commenterText.setStyle(sf::Text::Bold);
        commenterText.setPosition({ 60.f, yPos + 5.f });
        window.draw(commenterText);

        sf::Text commentText(font);
        commentText.setString(c->text);
        commentText.setCharacterSize(13);
        commentText.setFillColor(sf::Color(200, 200, 200));
        commentText.setPosition({ 60.f, yPos + 23.f });
        window.draw(commentText);

        yPos += 50.f;
        c = c->next;
        displayed++;
    }

    yPos += 15.f;
}