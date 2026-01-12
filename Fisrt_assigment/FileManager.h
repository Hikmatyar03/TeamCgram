#pragma once
#include <string>

class UserBST;
class User;
class Post;

class FileManager
{
private:
    std::string basePath;

public:
    explicit FileManager(const std::string& path = "data/");

    // user files
    void createUserFile(const std::string& username);
    void saveUserData(const User& user);
    void loadUserData(User& user);

    // global feed
    void appendToGlobalFeed(const User& user, const Post& post);
    void loadDummyFeed(UserBST& users);

    // system data
    void saveUsers(const UserBST& users) const;
    void loadUsers(UserBST& users);

    void savePosts(const UserBST& users) const;
    void loadPosts(UserBST& users);

    void saveConnections(const UserBST& users) const;
    void loadConnections(UserBST& users);

    void saveNotifications(const UserBST& users) const;
    void loadNotifications(UserBST& users);
};
