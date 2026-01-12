#include "App.h"
#include "Screen.h"
#include "FileManager.h"
#include "User.h"
#include "UserBST.h"
#include <iostream>

void App::loadData()
{
    fileManager.loadUsers(users);
    fileManager.loadConnections(users);
    fileManager.loadNotifications(users);
}

void App::saveData()
{
    fileManager.saveUsers(users);
    fileManager.saveConnections(users);
    fileManager.saveNotifications(users);
}

App::App()
    : currentUser(nullptr),
    currentScreen(nullptr),
    fileManager("data/")
{
    loadData();
    fileManager.loadDummyFeed(users);

    std::cout << "App initialized. Total users: " << users.getAllUsers().size() << std::endl;
}

bool App::registerUser(const std::string& username, const std::string& password)
{
    if (!users.registerUser(username, password))
        return false;

    User* u = users.getUserByUsername(username);
    fileManager.createUserFile(username);
    fileManager.saveUserData(*u);

    // Auto-follow all existing users
    for (User* other : users.getAllUsers())
    {
        if (other->userID != u->userID)
        {
            users.followUser(u->userID, other->userID);
        }
    }

    saveData();

    // Auto-login after registration
    currentUser = u;
    fileManager.loadUserData(*currentUser);

    return true;
}

bool App::login(const std::string& username, const std::string& password)
{
    User* user = users.getUserByUsername(username);
    if (!user || user->password != password)
        return false;

    currentUser = user;
    fileManager.loadUserData(*user);

    std::cout << "Login successful: " << currentUser->username << std::endl;

    // Auto-follow all users if not following anyone
    if (currentUser->following.empty())
    {
        for (User* u : users.getAllUsers())
        {
            if (u->userID != currentUser->userID)
            {
                users.followUser(currentUser->userID, u->userID);
            }
        }
    }

    return true;
}

User* App::getCurrentUser() const
{
    return currentUser;
}

std::vector<Post*> App::getFeed() const
{
    if (!currentUser) {
        std::cout << "No current user!" << std::endl;
        return {};
    }

    std::cout << "Getting feed for: " << currentUser->username << std::endl;
    std::cout << "Following: " << currentUser->following.size() << " users" << std::endl;

    auto feed = feedManager.generateFeed(currentUser, users);

    std::cout << "Feed size: " << feed.size() << std::endl;

    return feed;
}

void App::createPost(const std::string& text)
{
    if (!currentUser || text.empty()) return;

    Post* p = currentUser->posts.createPost(currentUser->userID, text);
    fileManager.saveUserData(*currentUser);
    fileManager.appendToGlobalFeed(*currentUser, *p);
}

void App::logout()
{
    currentUser = nullptr;
    saveData();
}

void App::setScreen(Screen* screen)
{
    if (currentScreen != nullptr) {
        delete currentScreen;
    }
    currentScreen = screen;
}

Screen* App::getScreen() const
{
    return currentScreen;
}

UserBST& App::getUsers()
{
    return users;
}

App::~App()
{
    saveData();
    delete currentScreen;
}