
#include "App.h"
#include <SFML/Graphics.hpp>

App::App() : window(sf::VideoMode({ 800,600 }), "Social Media App") {}

void App::run(){
    while (window.isOpen())
    {
        
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color::Black);
        window.display();
    }
}
