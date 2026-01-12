#include "NotificationScreen.h"

NotificationScreen::NotificationScreen(App& app) : app(app) {
    font.openFromFile("font / Inter_28pt - Regular.ttf");
}
void NotificationScreen::handleEvent(const sf::Event&, sf::RenderWindow&) {
    // No interaction yet
}
void NotificationScreen::update(sf::Time) {
    User* u = app.getCurrentUser();
    if (u)
        u->notifications.markAllRead();
}
void NotificationScreen::render(sf::RenderWindow& window) {
    User* u = app.getCurrentUser();
    if (!u) return;

    float y = 20.f;
    for (auto& n : u->notifications.getAllOrdered())
    {
        sf::Text t(font);          // ✅ construct with font
        t.setString(n.message);
        t.setPosition({ 20.f, y });

        window.draw(t);
        y += 30.f;
    }

}
