#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Screen.h"
#include "UserBST.h"
#include "NotificationSystem.h"

// Forward declarations
class Screen;

class App {
private:
    UserBST users;
    User* currentUser;
    Screen* currentScreen;
    NotificationManager notificationManager;

public:
    App();
    ~App();

    // Authentication
    bool login(const std::string& username, const std::string& password);
    bool registerUser(const std::string& username, const std::string& password);
    void logout();

    // Getters
    User* getCurrentUser() const { return currentUser; }
    UserBST& getUsers() { return users; }
    const UserBST& getUsers() const { return users; }
    Screen* getScreen() const { return currentScreen; }
    NotificationManager& getNotificationManager() { return notificationManager; }
    std::vector<Post*> getAllPosts() const;

    // Screen management
    void setScreen(Screen* screen);

    // Post operations
    Post* createPost(int userID, const std::string& content);
    void deletePost(int postID);

    // Social operations
    void followUser(int followerID, int followeeID);
    void unfollowUser(int followerID, int followeeID);
    void likePost(int userID, int postID);
    void unlikePost(int userID, int postID);
    void addComment(int postID, int userID, const std::string& content);
};