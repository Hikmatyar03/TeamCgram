#pragma once
#include "UserBST.h"
#include "FeedManager.h"
#include "HistoryStack.h"
#include "FileManager.h"
#include "Screen.h"

class Screen; // forward declaration

class App
{
public:
    App();
    ~App();
    void saveData();

    void setScreen(Screen* screen);
    Screen* getScreen() const;

    bool registerUser(const std::string& username, const std::string& password);
    bool login(const std::string& username, const std::string& password);
    void logout();

    User* getCurrentUser() const;
    UserBST& getUsers();
    std::vector<Post*> getFeed() const;

    void createPost(const std::string& text);
    
  
private:
 
    void loadData();
    User* currentUser = nullptr;
    Screen* currentScreen = nullptr;

    UserBST users;
    FeedManager feedManager;
    FileManager fileManager;
    HistoryStack history;
};
