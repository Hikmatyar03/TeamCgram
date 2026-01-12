
#pragma once
#include <SFML/Graphics.hpp>
#include "../core/UserManager.h"

class App {
public:
    sf::RenderWindow window;
    UserManager users;
    App();
    void run();
};
