//#include <SFML/Graphics.hpp>
//#include <fstream>
//#include <vector>
//#include <string>
//#include <iostream>
//#include <memory>
//#include <algorithm>
//#include <chrono>   // for delta-time
//
//struct ShapeData {
//    std::string type;
//    std::string name;
//    sf::Vector2f pos;
//    sf::Vector2f speed;
//    sf::Color color;
//    float size1 = 0.f;
//    float size2 = 0.f;
//};
//
//struct Player {
//    sf::RectangleShape shape;
//    sf::Text label;
//    float moveSpeed = 5.f;
//    int score = 0;
//    Player() = default;
//};
//
//struct Enemy {
//    std::unique_ptr<sf::Shape> shape;
//    sf::Text label;
//    sf::Vector2f velocity;
//    int hp = 3;
//    bool active = true;
//    Enemy() = default;
//};
//
//struct Bullet {
//    sf::RectangleShape shape;
//    sf::Vector2f velocity;
//    bool active = true;
//    Bullet() = default;
//};
//
//int main() {
//    // 1) Parse config
//    std::ifstream config("config_data.txt");
//    if (!config) {
//        std::cerr << "Error: config_data.txt not found.\n";
//        return 1;
//    }
//
//    std::string token;
//    unsigned int winW = 1280, winH = 720;
//    std::string windowTitle = "Shape Shooter";
//    std::string fontPath;
//    unsigned int fontSize = 16;
//    sf::Color fontColor = sf::Color::White;
//    std::vector<ShapeData> parsedShapes;
//
//    while (config >> token) {
//        if (token == "Window") {
//            config >> winW >> winH;
//        }
//        else if (token == "Font") {
//            int r, g, b;
//            config >> fontPath >> fontSize >> r >> g >> b;
//            fontColor = sf::Color(r, g, b);
//        }
//        else if (token == "Circle" || token == "Rectangle") {
//            ShapeData sd;
//            sd.type = token;
//            int r, g, b;
//            config >> sd.name >> sd.pos.x >> sd.pos.y >> sd.speed.x >> sd.speed.y >> r >> g >> b >> sd.size1;
//            if (token == "Rectangle") {
//                config >> sd.size2;
//            }
//            sd.color = sf::Color(r, g, b);
//            parsedShapes.push_back(sd);
//        }
//    }
//
//    // 2) Setup window & font
//    sf::RenderWindow window(sf::VideoMode({ winW, winH }), windowTitle);
//    window.setFramerateLimit(60);
//
//    sf::Font font;
//        if (!font.openFromFile(fontPath)) {
//            std::cerr << "Error: Could not load font.\n";
//            return -1;
//        }}
//
//    Player player;
//    std::vector<Enemy> enemies;
//    std::vector<Bullet> bullets;
//
//    bullets.reserve(100);   // anticipate bullets
//    enemies.reserve(parsedShapes.size());
//
//    if (!parsedShapes.empty()) {
//        const auto& pd = parsedShapes[0];
//        player.shape.setSize({ pd.size1, pd.size2 });
//        player.shape.setPosition(pd.pos);
//        player.shape.setFillColor(pd.color);
//        player.shape.setOrigin({ pd.size1 / 2.f, pd.size2 / 2.f });
//        player.label.setFont(font);
//        player.label.setString(pd.name);
//        player.label.setCharacterSize(fontSize);
//        player.label.setFillColor(fontColor);
//        player.moveSpeed = pd.speed.x;
//
//        for (size_t i = 1; i < parsedShapes.size(); ++i) {
//            const auto& s = parsedShapes[i];
//            Enemy e;
//            if (s.type == "Circle") {
//                auto c = std::make_unique<sf::CircleShape>(s.size1);
//                c->setOrigin({ s.size1, s.size1 });
//                e.shape = std::move(c);
//            }
//            else {
//                auto r = std::make_unique<sf::RectangleShape>(sf::Vector2f(s.size1, s.size2));
//                r->setOrigin({ s.size1 / 2.f, s.size2 / 2.f });
//                e.shape = std::move(r);
//            }
//            e.shape->setPosition(s.pos);
//            e.shape->setFillColor(s.color);
//            e.velocity = s.speed;
//            e.label.setFont(font);
//            e.label.setString(s.name);
//            e.label.setCharacterSize(fontSize);
//            e.label.setFillColor(fontColor);
//            enemies.emplace_back(std::move(e));
//        }
//    }
//
//    // UI texts
//    sf::Text scoreText(font, "Score: 0", 24);
//    scoreText.setFillColor(sf::Color::White);
//    scoreText.setPosition({ 10.f, 10.f });
//
//    sf::Text winText(font, "YOU WIN!", 64);
//    winText.setFillColor(sf::Color::Green);
//    auto wb = winText.getLocalBounds();
//    winText.setOrigin({ wb.width / 2.f, wb.height / 2.f });
//    winText.setPosition({ winW / 2.f, winH / 2.f });
//
//    bool gameOver = false;
//
//    auto centerTextOnShape = [&](sf::Text& text, const sf::Shape& shape) {
//        auto tb = text.getLocalBounds();
//        text.setOrigin({ tb.width / 2.f, tb.height / 2.f });
//        text.setPosition(shape.getPosition());
//        };
//
//    sf::Clock deltaClock;
//    sf::Clock shootClock;
//    const sf::Time shootCooldown = sf::seconds(0.2f);
//
//    // 5) Main loop
//    while (window.isOpen()) {
//        // delta time
//        sf::Time dt = deltaClock.restart();
//        float delta = dt.asSeconds();
//
//        // event polling
//        while (auto eventOpt = window.pollEvent()) {
//            const sf::Event& event = *eventOpt;
//            if (event.type == sf::Event::Closed) {
//                window.close();
//            }
//        }
//
//        if (!gameOver) {
//            // Player movement: use dt for frame independent movement
//            sf::Vector2f moveVec{ 0.f, 0.f };
//            const float ms = player.moveSpeed * delta;
//            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A)) moveVec.x -= ms;
//            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D)) moveVec.x += ms;
//            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W)) moveVec.y -= ms;
//            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S)) moveVec.y += ms;
//            player.shape.move(moveVec);
//
//            // Keep player in bounds
//            auto pb = player.shape.getGlobalBounds();
//            if (player.shape.getPosition().x - pb.size.x / 2.f < 0) player.shape.setPosition(pb.size.x / 2.f, player.shape.getPosition().y);
//            if (player.shape.getPosition().x + pb.size.x / 2.f > winW) player.shape.setPosition(winW - pb.size.x / 2.f, player.shape.getPosition().y);
//            if (player.shape.getPosition().y - pb.size.y / 2.f < 0) player.shape.setPosition(player.shape.getPosition().x, pb.size.y / 2.f);
//            if (player.shape.getPosition().y + pb.size.y / 2.f > winH) player.shape.setPosition(player.shape.getPosition().x, winH - pb.size.y / 2.f);
//
//            // Shooting
//            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && shootClock.getElapsedTime() >= shootCooldown) {
//                shootClock.restart();
//                Bullet b;
//                b.shape.setSize({ 10.f, 20.f });
//                b.shape.setFillColor(sf::Color::Yellow);
//                b.shape.setOrigin({ 5.f, 10.f });
//                b.shape.setPosition(player.shape.getPosition());
//                b.velocity = { 0.f, -600.f };   // speed units per second
//                bullets.emplace_back(std::move(b));
//            }
//
//            // Move bullets
//            for (auto& b : bullets) {
//                b.shape.move(b.velocity * delta);
//                if (b.shape.getPosition().y < 0.f) {
//                    b.active = false;
//                }
//            }
//
//            // Move enemies & bounce
//            for (auto& e : enemies) {
//                e.shape->move(e.velocity * delta);
//                auto gb = e.shape->getGlobalBounds();
//                if (gb.position.x <= 0.f || (gb.position.x + gb.size.x) >= winW) {
//                    e.velocity.x = -e.velocity.x;
//                }
//                if (gb.position.y <= 0.f || (gb.position.y + gb.size.y) >= winH) {
//                    e.velocity.y = -e.velocity.y;
//                }
//            }
//
//            // Collision detection
//            for (auto& b : bullets) {
//                if (!b.active) continue;
//                for (auto& e : enemies) {
//                    if (!e.active) continue;
//                    if (b.shape.getGlobalBounds().intersects(e.shape->getGlobalBounds())) {
//                        b.active = false;
//                        e.hp--;
//                        if (e.hp <= 0) {
//                            e.active = false;
//                            player.score += 10;
//                        }
//                        break;
//                    }
//                }
//            }
//
//            // Cleanup inactive
//            bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
//                [](const Bullet& b) { return !b.active; }), bullets.end());
//            enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
//                [](const Enemy& e) { return !e.active; }), enemies.end());
//
//            // Update labels
//            centerTextOnShape(player.label, player.shape);
//            for (auto& e : enemies) {
//                centerTextOnShape(e.label, *e.shape);
//            }
//
//            if (enemies.empty()) {
//                gameOver = true;
//            }
//        }
//
//        // UI update
//        scoreText.setString("Score: " + std::to_string(player.score));
//
//        // Render
//        window.clear(sf::Color(10, 20, 40));
//        window.draw(player.shape);
//        window.draw(player.label);
//        for (auto& e : enemies) {
//            window.draw(*e.shape);
//            window.draw(e.label);
//        }
//        for (auto& b : bullets) {
//            window.draw(b.shape);
//        }
//
//        window.draw(scoreText);
//        if (gameOver) {
//            window.draw(winText);
//        }
//        window.display();
//    }
//
//    return 0;
//}
