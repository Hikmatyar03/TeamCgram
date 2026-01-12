#include "FeedScreen.h"
#include "LoginScreen.h"
#include <iostream>
#include <algorithm>

FeedScreen::FeedScreen(App& app)
    : app(app),
    font(),
    header(font),
    logoutText(font),
    addPostText(font),
    postInputText(font),
    commentInputText(font),
    scrollOffset(0.f),
    targetScroll(0.f),
    addingPost(false),
    addingComment(false),
    activePost(nullptr)
{
    if (!font.openFromFile("font/Inter_28pt-Regular.ttf"))
    {
        std::cerr << "Font load failed\n";
    }

    header.setString("Your Feed");
    header.setCharacterSize(28);
    header.setFillColor(sf::Color::White);
    header.setPosition({ 20.f, 10.f });

    logoutBtn.setSize({ 120.f, 35.f });
    logoutBtn.setPosition({ 650.f, 10.f });
    logoutBtn.setFillColor(sf::Color(200, 80, 80));

    logoutText.setString("Logout");
    logoutText.setCharacterSize(20);
    logoutText.setFillColor(sf::Color::White);
    logoutText.setPosition({ 680.f, 15.f });

    addPostBtn.setSize({ 120.f, 35.f });
    addPostBtn.setPosition({ 500.f, 10.f });
    addPostBtn.setFillColor(sf::Color(80, 160, 80));

    addPostText.setString("Add Post");
    addPostText.setCharacterSize(20);
    addPostText.setFillColor(sf::Color::White);
    addPostText.setPosition({ 520.f, 15.f });

    postInputText.setCharacterSize(16);
    postInputText.setFillColor(sf::Color::White);
    postInputText.setPosition({ 20.f, 60.f });

    commentInputText.setCharacterSize(16);
    commentInputText.setFillColor(sf::Color::White);
    commentInputText.setPosition({ 20.f, 60.f });
}

void FeedScreen::handleEvent(const sf::Event& event, sf::RenderWindow&)
{
    // Safety check
    if (!app.getCurrentUser())
    {
        return;
    }

    // Smooth scroll
    if (const auto* wheel = event.getIf<sf::Event::MouseWheelScrolled>())
    {
        targetScroll += wheel->delta * 120.f;
        targetScroll = std::clamp(targetScroll, -2000.f, 0.f);
    }

    // Mouse clicks
    if (const auto* mouse = event.getIf<sf::Event::MouseButtonPressed>())
    {
        sf::Vector2f mousePos(
            static_cast<float>(mouse->position.x),
            static_cast<float>(mouse->position.y)
        );

        if (logoutBtn.getGlobalBounds().contains(mousePos))
        {
            app.logout();
            app.setScreen(new LoginScreen(app));
            return;
        }

        if (addPostBtn.getGlobalBounds().contains(mousePos))
        {
            addingPost = true;
            addingComment = false;
            postInput.clear();
            return;
        }

        // Post interaction
        float y = 100.f + scrollOffset;
        auto feed = app.getFeed();

        for (Post* post : feed)
        {
            int commentCount = 0;
            for (Comment* c = post->comments; c; c = c->next) commentCount++;

            sf::FloatRect likeBtn({ 480.f, y + 70.f }, { 70.f, 25.f });
            sf::FloatRect commentBtn({ 400.f, y + 70.f }, { 70.f, 25.f });

            if (likeBtn.contains(mousePos))
            {
                post->like(app.getCurrentUser()->userID);
                return;
            }

            if (commentBtn.contains(mousePos))
            {
                addingComment = true;
                addingPost = false;
                activePost = post;
                commentInput.clear();
                return;
            }

            y += 120.f + commentCount * 18.f + 20.f;
        }
    }

    // Text input
    if (const auto* text = event.getIf<sf::Event::TextEntered>())
    {
        if (text->unicode == 8) // Backspace
        {
            if (addingPost && !postInput.empty()) postInput.pop_back();
            if (addingComment && !commentInput.empty()) commentInput.pop_back();
        }
        else if (text->unicode >= 32 && text->unicode < 128) // Printable characters
        {
            if (addingPost) postInput += static_cast<char>(text->unicode);
            if (addingComment) commentInput += static_cast<char>(text->unicode);
        }
    }

    // Submit
    if (const auto* key = event.getIf<sf::Event::KeyPressed>())
    {
        if (key->scancode == sf::Keyboard::Scancode::Enter)
        {
            if (addingPost && !postInput.empty())
            {
                app.createPost(postInput);
                postInput.clear();
                addingPost = false;
            }
            else if (addingComment && activePost && !commentInput.empty())
            {
                activePost->addComment(
                    app.getCurrentUser()->userID,
                    commentInput
                );
                commentInput.clear();
                addingComment = false;
                activePost = nullptr;
            }
        }
        else if (key->scancode == sf::Keyboard::Scancode::Escape)
        {
            addingPost = false;
            addingComment = false;
            postInput.clear();
            commentInput.clear();
        }
    }
}

void FeedScreen::update(sf::Time dt)
{
    if (!app.getCurrentUser()) return;

    // Smooth scrolling
    scrollOffset += (targetScroll - scrollOffset) * 12.f * dt.asSeconds();
}

void FeedScreen::render(sf::RenderWindow& window)
{
    // Safety check - prevent crash
    if (!app.getCurrentUser())
    {
        sf::Text errorText(font);
        errorText.setString("Please log in");
        errorText.setCharacterSize(24);
        errorText.setFillColor(sf::Color::Red);
        errorText.setPosition({ 300.f, 250.f });
        window.draw(errorText);
        return;
    }

    // Draw header elements
    window.draw(header);
    window.draw(logoutBtn);
    window.draw(logoutText);
    window.draw(addPostBtn);
    window.draw(addPostText);

    // Input prompts
    if (addingPost)
    {
        sf::RectangleShape inputBox({ 760.f, 30.f });
        inputBox.setPosition({ 20.f, 55.f });
        inputBox.setFillColor(sf::Color(60, 60, 60));
        inputBox.setOutlineColor(sf::Color::Green);
        inputBox.setOutlineThickness(2.f);
        window.draw(inputBox);

        postInputText.setString("New Post: " + postInput + "_");
        window.draw(postInputText);
    }

    if (addingComment)
    {
        sf::RectangleShape inputBox({ 760.f, 30.f });
        inputBox.setPosition({ 20.f, 55.f });
        inputBox.setFillColor(sf::Color(60, 60, 60));
        inputBox.setOutlineColor(sf::Color::Blue);
        inputBox.setOutlineThickness(2.f);
        window.draw(inputBox);

        commentInputText.setString("Comment: " + commentInput + "_");
        window.draw(commentInputText);
    }

    // Get feed
    auto feed = app.getFeed();

    // Show message if feed is empty
    if (feed.empty())
    {
        sf::Text emptyText(font);
        emptyText.setString("No posts yet.\nFollow users or create your first post!");
        emptyText.setCharacterSize(20);
        emptyText.setFillColor(sf::Color(150, 150, 150));
        emptyText.setPosition({ 250.f, 250.f });
        window.draw(emptyText);
        return;
    }

    // Draw posts
    float y = 100.f + scrollOffset;

    for (Post* post : feed)
    {
        // Count comments
        int commentCount = 0;
        for (Comment* c = post->comments; c; c = c->next) commentCount++;

        // Post card
        sf::RectangleShape card({ 760.f, 120.f + commentCount * 20.f });
        card.setPosition({ 20.f, y });
        card.setFillColor(sf::Color(50, 50, 50));
        card.setOutlineColor(sf::Color(100, 100, 100));
        card.setOutlineThickness(1.f);
        window.draw(card);

        // Post author (you'll need to add username to post or get it from UserBST)
        sf::Text authorText(font);
        User* author = app.getUsers().getUserByID(post->userID);
        authorText.setString(author ? author->username : "Unknown");
        authorText.setCharacterSize(16);
        authorText.setFillColor(sf::Color(100, 200, 255));
        authorText.setStyle(sf::Text::Bold);
        authorText.setPosition({ 30.f, y + 10.f });
        window.draw(authorText);

        // Post content
        sf::Text content(font);
        content.setString(post->content);
        content.setCharacterSize(18);
        content.setFillColor(sf::Color::White);
        content.setPosition({ 30.f, y + 35.f });
        window.draw(content);

        // Likes
        sf::Text likes(font);
        likes.setString("❤ " + std::to_string(post->likes));
        likes.setCharacterSize(16);
        likes.setFillColor(sf::Color(255, 100, 100));
        likes.setPosition({ 30.f, y + 75.f });
        window.draw(likes);

        // Like button
        sf::RectangleShape likeBtn({ 70.f, 25.f });
        likeBtn.setPosition({ 480.f, y + 70.f });
        likeBtn.setFillColor(sf::Color(200, 100, 100));
        window.draw(likeBtn);

        sf::Text likeText(font);
        likeText.setString("Like");
        likeText.setCharacterSize(16);
        likeText.setFillColor(sf::Color::White);
        likeText.setPosition({ 495.f, y + 73.f });
        window.draw(likeText);

        // Comment button
        sf::RectangleShape commentBtn({ 90.f, 25.f });
        commentBtn.setPosition({ 380.f, y + 70.f });
        commentBtn.setFillColor(sf::Color(120, 120, 200));
        window.draw(commentBtn);

        sf::Text commentText(font);
        commentText.setString("Comment");
        commentText.setCharacterSize(16);
        commentText.setFillColor(sf::Color::White);
        commentText.setPosition({ 385.f, y + 73.f });
        window.draw(commentText);

        // Display comments
        float commentY = y + 105.f;
        for (Comment* c = post->comments; c; c = c->next)
        {
            User* commentAuthor = app.getUsers().getUserByID(c->userID);
            sf::Text ct(font);
            ct.setCharacterSize(14);
            ct.setFillColor(sf::Color(200, 200, 200));
            ct.setString((commentAuthor ? commentAuthor->username : "User") + ": " + c->text);
            ct.setPosition({ 40.f, commentY });
            window.draw(ct);
            commentY += 20.f;
        }

        y += card.getSize().y + 15.f;
    }
}