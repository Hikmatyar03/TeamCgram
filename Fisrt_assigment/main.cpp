#include <SFML/Graphics.hpp>
#include "App.h"
#include "LoginScreen.h"

int main()
{
    sf::RenderWindow window(
        sf::VideoMode({ 800, 600 }),
        "Social Media System"
    );
    window.setFramerateLimit(60);

    App app;
    app.setScreen(new LoginScreen(app));

    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Time delta = clock.restart();

        // ✅ SFML 3 event loop
        while (const auto event = window.pollEvent())
        {
            if (event->getIf<sf::Event::Closed>())
                window.close();

            if (app.getScreen())
                app.getScreen()->handleEvent(*event, window);
        }

        if (app.getScreen())
            app.getScreen()->update(delta);

        window.clear(sf::Color(30, 30, 30));

        if (app.getScreen())
            app.getScreen()->render(window);

        window.display();
    }

    return 0;
}
