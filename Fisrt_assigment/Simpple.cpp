/*
//#include <SFML/Graphics.hpp>
//#include <string>
//#include <vector>
//#include <optional>
//#include <cstdint>
//
//// ================= USER (BST) =================
//struct User {
//    int id;
//    std::string username;
//    std::string password;
//    User* left;
//    User* right;
//
//    User(int i, std::string u, std::string p)
//        : id(i), username(u), password(p), left(nullptr), right(nullptr) {
//    }
//};
//
//User* insertUser(User* root, User* u) {
//    if (!root) return u;
//    if (u->id < root->id)
//        root->left = insertUser(root->left, u);
//    else
//        root->right = insertUser(root->right, u);
//    return root;
//}
//
//User* login(User* root, const std::string& u, const std::string& p) {
//    if (!root) return nullptr;
//    if (root->username == u && root->password == p)
//        return root;
//    User* l = login(root->left, u, p);
//    return l ? l : login(root->right, u, p);
//}
//
//// ================= POST =================
//struct Post {
//    std::string text;
//};
//
//// ================= SCREEN =================
//enum class Screen { LOGIN, FEED };
//enum class Focus { USERNAME, PASSWORD };
//
//int main() {
//    sf::RenderWindow window(sf::VideoMode({800, 600}), "Social Media System");
//
//    sf::Font font;
//    if (!font.openFromFile("font/Inter_28pt-Regular.ttf"))
//        return -1;
//
//    // ===== USERS =====
//    User* users = nullptr;
//    users = insertUser(users, new User(1, "admin", "admin123"));
//
//    // ===== STATE =====
//    Screen screen = Screen::LOGIN;
//    Focus focus = Focus::USERNAME;
//
//    std::string username;
//    std::string password;
//
//    std::vector<Post> feed = {
//        {"Welcome to the feed"},
//        {"This is a DSA project"},
//        {"SFML 3.0 working correctly"}
//    };
//
//    int selected = 0;
//
//    // ===== UI =====
//    sf::Text title(font);
//    title.setCharacterSize(28);
//    title.setString("LOGIN");
//    title.setPosition(sf::Vector2f{ 320.f, 80.f });
//
//    sf::Text userText(font);
//    userText.setCharacterSize(18);
//    userText.setPosition(sf::Vector2f{ 280.f, 200.f });
//
//    sf::Text passText(font);
//    passText.setCharacterSize(18);
//    passText.setPosition(sf::Vector2f{ 280.f, 250.f });
//
//    // ================= LOOP =================
//    while (window.isOpen()) {
//
//        while (std::optional<sf::Event> event = window.pollEvent()) {
//
//            // CLOSE
//            if (event->is<sf::Event::Closed>())
//                window.close();
//
//            // TEXT INPUT
//            if (event->is<sf::Event::TextEntered>() && screen == Screen::LOGIN) {
//                if (const auto* textEvent = event->getIf<sf::Event::TextEntered>())
//                {
//                    std::uint32_t ch = textEvent->unicode;
//               
//
//
//                if (ch == 8) { // backspace
//                    if (focus == Focus::USERNAME && !username.empty())
//                        username.pop_back();
//                    else if (focus == Focus::PASSWORD && !password.empty())
//                        password.pop_back();
//                }
//                else if (ch >= 32 && ch <= 126) {
//                    if (focus == Focus::USERNAME)
//                        username += static_cast<char>(ch);
//                    else
//                        password += static_cast<char>(ch);
//                }
//            }
//            }
//
//            // KEYBOARD
//            if (event->is<sf::Event::KeyPressed>()) {
//                auto key = event->getIf<sf::Event::KeyPressed>()->code;
//
//
//                using K = sf::Keyboard::Key;
//
//                if (screen == Screen::LOGIN) {
//                    if (key == K::Tab)
//                        focus = (focus == Focus::USERNAME)
//                        ? Focus::PASSWORD
//                        : Focus::USERNAME;
//
//                    if (key == K::Enter) {
//                        if (login(users, username, password)) {
//                            screen = Screen::FEED;
//                            title.setString("FEED");
//                        }
//                        username.clear();
//                        password.clear();
//                        focus = Focus::USERNAME;
//                    }
//                }
//                else if (screen == Screen::FEED) {
//                    if (key == K::Down)
//                        selected = std::min(selected + 1, (int)feed.size() - 1);
//                    if (key == K::Up)
//                        selected = std::max(selected - 1, 0);
//                    if (key == K::Escape) {
//                        screen = Screen::LOGIN;
//                        title.setString("LOGIN");
//                        selected = 0;
//                    }
//                }
//            }
//        }
//
//        // ================= DRAW =================
//        window.clear(sf::Color::Black);
//        window.draw(title);
//
//        if (screen == Screen::LOGIN) {
//            userText.setString("User: " + username);
//            passText.setString("Pass: " + std::string(password.size(), '*'));
//
//            window.draw(userText);
//            window.draw(passText);
//        }
//        else if (screen == Screen::FEED) {
//            float y = 180.f;
//            for (size_t i = 0; i < feed.size(); ++i) {
//                sf::RectangleShape card(sf::Vector2f{ 520.f, 40.f });
//                card.setPosition(sf::Vector2f{ 140.f, y });
//                card.setFillColor(
//                    i == selected
//                    ? sf::Color(200, 190, 255)
//                    : sf::Color::White
//                );
//
//                sf::Text post(font);
//                post.setCharacterSize(16);
//                post.setString(feed[i].text);
//                post.setPosition(sf::Vector2f{ 160.f, y + 10.f });
//                post.setFillColor(sf::Color::Black);
//
//                window.draw(card);
//                window.draw(post);
//                y += 55.f;
//            }
//        }
//
//        window.display();
//    }
//
//    return 0;
//}

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdint>

// ================= BST USER SYSTEM =================
struct User {
    std::string username, password;
    User* left;
    User* right;
    User(const std::string& u, const std::string& p)
        : username(u), password(p), left(nullptr), right(nullptr) {
    }
};

User* insertUser(User* root, const std::string& u, const std::string& p) {
    if (!root) return new User(u, p);
    if (u < root->username) root->left = insertUser(root->left, u, p);
    else if (u > root->username) root->right = insertUser(root->right, u, p);
    return root;
}

User* findUser(User* root, const std::string& u, const std::string& p) {
    if (!root) return nullptr;
    if (u == root->username && p == root->password) return root;
    if (u < root->username) return findUser(root->left, u, p);
    return findUser(root->right, u, p);
}

// ================= DATA =================
struct Post {
    std::string text;
    int likes = 0;
};

enum class Screen { LOGIN, SIGNUP, FEED };
enum class Focus { USERNAME, PASSWORD };

int main() {
    sf::RenderWindow window(sf::VideoMode({ 760, 580 }), "SFML 3.0 Social App");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.openFromFile("font/Inter_28pt-Regular.ttf")) return -1;

    // ===== USERS (BST) =====
    User* userRoot = nullptr;
    userRoot = insertUser(userRoot, "admin", "admin123");

    // ===== STATE =====
    Screen screen = Screen::LOGIN;
    Focus focus = Focus::USERNAME;
    std::string username, password;
    std::string errorMsg;
    bool darkMode = false;

    // ===== POSTS =====
    std::vector<Post> posts = {
        {"Welcome to the feed"},
        {"Login & Signup working"},
        {"Animated gradient UI"},
        {"Glassmorphism cards"}
    };
    int selectedPost = 0;

    // ===== UI TEXT =====
    sf::Text title(font, "LOGIN", 32);
    title.setPosition({ 310, 60 });

    sf::Text switchText(font, "Press S to Sign Up", 14);
    switchText.setPosition({ 300, 110 });

    sf::Text userLabel(font, "Username:", 18);
    userLabel.setPosition({ 220, 220 });

    sf::Text passLabel(font, "Password:", 18);
    passLabel.setPosition({ 220, 270 });

    sf::Text userText(font, "", 18);
    userText.setPosition({ 340, 220 });

    sf::Text passText(font, "", 18);
    passText.setPosition({ 340, 270 });

    sf::Text errorText(font, "", 16);
    errorText.setPosition({ 280, 320 });
    errorText.setFillColor(sf::Color(200, 50, 50));

    // ================= LOOP =================
    float anim = 0.f;

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {

            if (event->is<sf::Event::Closed>())
                window.close();

            // -------- TEXT INPUT --------
            if (screen != Screen::FEED) {
                if (const auto* te = event->getIf<sf::Event::TextEntered>()) {
                    std::uint32_t ch = te->unicode;

                    if (ch == 8) { // backspace
                        if (focus == Focus::USERNAME && !username.empty())
                            username.pop_back();
                        else if (focus == Focus::PASSWORD && !password.empty())
                            password.pop_back();
                    }
                    else if (ch >= 32 && ch <= 126) {
                        if (focus == Focus::USERNAME)
                            username += static_cast<char>(ch);
                        else
                            password += static_cast<char>(ch);
                    }
                }
            }

            // -------- KEY INPUT --------
            if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                using Key = sf::Keyboard::Key;

                if (key->code == Key::Q)
                    darkMode = !darkMode;

                if (screen != Screen::FEED) {
                    if (key->code == Key::Tab)
                        focus = (focus == Focus::USERNAME)
                        ? Focus::PASSWORD
                        : Focus::USERNAME;

                    if (key->code == Key::Enter) {
                        errorMsg.clear();

                        if (screen == Screen::LOGIN) {
                            if (findUser(userRoot, username, password)) {
                                screen = Screen::FEED;
                                title.setString("FEED");
                            }
                            else {
                                errorMsg = "Invalid username or password";
                            }
                        }
                        else if (screen == Screen::SIGNUP) {
                            userRoot = insertUser(userRoot, username, password);
                            errorMsg = "Signup successful. Please login.";
                            screen = Screen::LOGIN;
                            title.setString("LOGIN");
                        }

                        username.clear();
                        password.clear();
                        focus = Focus::USERNAME;
                    }

                    if (key->code == Key::S) {
                        screen = Screen::SIGNUP;
                        title.setString("SIGN UP");
                        errorMsg.clear();
                    }

                    if (key->code == Key::L) {
                        screen = Screen::LOGIN;
                        title.setString("LOGIN");
                        errorMsg.clear();
                    }
                }
                else {
                    if (key->code == Key::Down)
                        selectedPost = std::min(selectedPost + 1, (int)posts.size() - 1);
                    if (key->code == Key::Up)
                        selectedPost = std::max(selectedPost - 1, 0);
                    if (key->code == Key::L)
                        posts[selectedPost].likes++;
                    if (key->code == Key::Escape) {
                        screen = Screen::LOGIN;
                        title.setString("LOGIN");
                        selectedPost = 0;
                    }
                }
            }
        }

        // ================= ANIMATED GRADIENT =================
        anim += 0.02f;
        float shift = std::sin(anim) * 8.f;

        sf::Color g1 = darkMode ? sf::Color(45, 40, 70) : sf::Color(245, 240, 255);
        sf::Color g2 = darkMode ? sf::Color(40, 35, 65) : sf::Color(230, 220, 245);
        sf::Color g3 = darkMode ? sf::Color(35, 30, 60) : sf::Color(215, 205, 235);
        sf::Color g4 = darkMode ? sf::Color(30, 25, 55) : sf::Color(200, 190, 225);

        sf::RectangleShape bg1({ 760, 145 }); bg1.setPosition({ 0, 0 + shift }); bg1.setFillColor(g1);
        sf::RectangleShape bg2({ 760, 145 }); bg2.setPosition({ 0, 145 + shift }); bg2.setFillColor(g2);
        sf::RectangleShape bg3({ 760, 145 }); bg3.setPosition({ 0, 290 + shift }); bg3.setFillColor(g3);
        sf::RectangleShape bg4({ 760, 145 }); bg4.setPosition({ 0, 435 + shift }); bg4.setFillColor(g4);

        window.clear();
        window.draw(bg1);
        window.draw(bg2);
        window.draw(bg3);
        window.draw(bg4);

        // ================= DRAW =================
        window.draw(title);

        if (screen != Screen::FEED) {
            // Glass panel
            sf::RectangleShape glass({ 420, 220 });
            glass.setPosition({ 170, 190 });
            glass.setFillColor(sf::Color(255, 255, 255, darkMode ? 40 : 130));
            glass.setOutlineThickness(1);
            glass.setOutlineColor(sf::Color(255, 255, 255, 120));

            userText.setString(username);
            passText.setString(std::string(password.size(), '*'));
            errorText.setString(errorMsg);

            window.draw(glass);
            window.draw(userLabel);
            window.draw(passLabel);
            window.draw(userText);
            window.draw(passText);
            window.draw(errorText);
        }
        else {
            float y = 160;
            for (size_t i = 0; i < posts.size(); ++i) {
                sf::RectangleShape card({ 520, 44 });
                card.setPosition({ 120, y });
                card.setFillColor(sf::Color(255, 255, 255, darkMode ? 35 : 140));
                if ((int)i == selectedPost) {
                    card.setOutlineThickness(2);
                    card.setOutlineColor(sf::Color(150, 120, 255));
                }

                sf::Text post(font,
                    posts[i].text + "  ❤ " + std::to_string(posts[i].likes),
                    18);
                post.setPosition({ 135, y + 10 });
                post.setFillColor(darkMode ? sf::Color::White : sf::Color(40, 40, 70));

                window.draw(card);
                window.draw(post);
                y += 55;
            }
        }

        window.display();
    }

    return 0;
}

*/
