#include "ProfileScreen.h"


ProfileScreen::ProfileScreen(App& app, int userID)
    : app(app),
    viewedUserID(userID),
    header(font),        // ✅ sf::Text constructed with font
    followText(font)     // ✅ sf::Text constructed with font
{
    // ✅ FIXED FONT PATH
    font.openFromFile("font/Inter_28pt-Regular.ttf");

    header.setCharacterSize(24);
    header.setPosition({ 20.f, 20.f });

    followBtn.setSize({ 140.f, 40.f });
    followBtn.setPosition({ 20.f, 70.f });
    followBtn.setFillColor(sf::Color(100, 180, 100));

    followText.setPosition({ 35.f, 75.f });

}

void ProfileScreen::handleEvent(const sf::Event& event, sf::RenderWindow&)
{
    if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>())
    {
        const sf::Vector2f mousePos{
            static_cast<float>(mousePressed->position.x),
            static_cast<float>(mousePressed->position.y)
        };

        if (followBtn.getGlobalBounds().contains(mousePos))
        {
            User* me = app.getCurrentUser();
            User* target = app.getUsers().getUserByID(viewedUserID);

            if (me && target)
            {
                if (me->isFollowing(viewedUserID))
                    app.getUsers().unfollowUser(me->userID, viewedUserID);
                else
                    app.getUsers().followUser(me->userID, viewedUserID);
            }
        }
    }
}




void ProfileScreen::update(sf::Time)
{
    User* u = app.getUsers().getUserByID(viewedUserID);
    User* me = app.getCurrentUser();

    if (!u) return;

    header.setString(
        u->username + "\nFollowers: " +
        std::to_string(u->followers.size())
    );

    if (me && me->isFollowing(viewedUserID))
        followText.setString("Unfollow");
    else
        followText.setString("Follow");
}

void ProfileScreen::render(sf::RenderWindow& window)
{
    window.draw(header);
    window.draw(followBtn);
    window.draw(followText);
}
