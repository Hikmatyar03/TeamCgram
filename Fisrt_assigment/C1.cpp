//#include <SFML/Graphics.hpp>
//#include <fstream>
//#include <sstream>
//#include <vector>
//#include <string>
//#include <iostream>
//#include <optional>
//#include <memory>
//
//// Struct to hold parsed shape data
//struct ShapeData {
//    std::string type;
//    std::string name;
//    sf::Vector2f pos;
//    sf::Vector2f speed;
//    sf::Color color;
//    float size1 = 0;  // radius (for circle) or width (for rectangle)
//    float size2 = 0;  // height (for rectangle only)
//};
//
//int main() {
//    std::ifstream config("config_data.txt");
//    if (!config) {
//        std::cerr << "Error: Config file not found.\n";
//        return 1;
//    }
//
//    std::string token;
//    unsigned int winW = 800, winH = 600;
//    std::string windowTitle = "Window";
//    std::string fontPath;
//    unsigned int fontSize = 16;
//    sf::Color fontColor = sf::Color::White;
//
//    std::vector<ShapeData> shapes;
//
//    // ---- Parse Config ----
//    while (config >> token) {
//        if (token == "Window") {
//            config >> winW >> winH;
//        }
//        else if (token == "Font") {
//            int r, g, b;
//            config >> fontPath >> fontSize >> r >> g >> b;
//            fontColor = sf::Color(r, g, b);
//        }
//        else if (token == "Circle") {
//            ShapeData sd;
//            sd.type = token;
//            int r, g, b;
//            config >> sd.name
//                >> sd.pos.x >> sd.pos.y
//                >> sd.speed.x >> sd.speed.y
//                >> r >> g >> b
//                >> sd.size1;          
//            sd.color = sf::Color(r, g, b);
//            shapes.push_back(sd);
//        }
//        else if (token == "Rectangle") {
//            ShapeData sd;
//            sd.type = token;
//            int r, g, b;
//            config >> sd.name
//                >> sd.pos.x >> sd.pos.y
//                >> sd.speed.x >> sd.speed.y
//                >> r >> g >> b
//                >> sd.size1 >> sd.size2;  
//            sd.color = sf::Color(r, g, b);
//            shapes.push_back(sd);
//        }
//    }
//
//    sf::RenderWindow window(sf::VideoMode({ winW, winH }), windowTitle);
//   /* window.setFramerateLimit();*/
//
//    // ---- Load Font ----
//    sf::Font font;
//    if (!font.openFromFile(fontPath)) {
//        std::cerr << "Error: Could not load font.\n";
//        return -1;
//    }
//
//    // ---- Create SFML Shape Objects ----
//    std::vector<std::unique_ptr<sf::Shape>> drawableShapes;
//    std::vector<sf::Vector2f> speeds;
//    std::vector<sf::Text> labels;
//
//    for (const auto& s : shapes) {
//        if (s.type == "Circle") {
//            auto c = std::make_unique<sf::CircleShape>(s.size1); // size1 = radius
//            c->setPosition(s.pos);
//            c->setFillColor(s.color);
//            drawableShapes.push_back(std::move(c));
//        }
//        else if (s.type == "Rectangle") {
//            auto r = std::make_unique<sf::RectangleShape>(sf::Vector2f(s.size1, s.size2));
//            r->setPosition(s.pos);
//            r->setFillColor(s.color);
//            drawableShapes.push_back(std::move(r));
//        }
//
//        speeds.push_back(s.speed);
//
//        sf::Text text(font);
//        text.setString(s.name);
//        text.setCharacterSize(fontSize);
//        text.setFillColor(fontColor);
//        labels.push_back(text);
//    }
//
//    // ---- Helper: Center Text inside Shape ----
//    auto centerTextInShape = [](sf::Text& text, const sf::Shape& shape) {
//
//        sf::FloatRect textBounds = text.getLocalBounds();
//
//
//        text.setOrigin(sf::Vector2f(
//            textBounds.position.x + textBounds.size.x / 2.0f,
//            textBounds.position.y + textBounds.size.y / 2.0f
//        ));
//
//
//        sf::FloatRect shapeBounds = shape.getGlobalBounds();
//
//
//        text.setPosition(sf::Vector2f(
//            shapeBounds.position.x + shapeBounds.size.x / 2.0f,
//            shapeBounds.position.y + shapeBounds.size.y / 2.0f
//        ));
//        };
//
//    // ---- Main Loop ----
//    while (window.isOpen()) {
//        while (const std::optional event = window.pollEvent()) {
//            if (event->is<sf::Event::Closed>())
//                window.close();
//        }
//
//        // Move shapes and handle bouncing
//        for (size_t i = 0; i < drawableShapes.size(); ++i) {
//            auto& shape = drawableShapes[i];
//            shape->move(speeds[i]);
//
//            sf::FloatRect bounds = shape->getGlobalBounds();
//
//            // Bounce on X axis
//            if (bounds.position.x <= 0 || bounds.position.x + bounds.size.x >= winW)
//                speeds[i].x = -speeds[i].x;
//
//            // Bounce on Y axis
//            if (bounds.position.y <= 0 || bounds.position.y + bounds.size.y >= winH)
//                speeds[i].y = -speeds[i].y;
//
//            centerTextInShape(labels[i], *shape);
//        }
//
//        // ---- Render ----
//        window.clear();
//        for (size_t i = 0; i < drawableShapes.size(); ++i) {
//            window.draw(*drawableShapes[i]);
//            window.draw(labels[i]);
//        }
//        window.display();
//    }
//
//    return 0;
//}
