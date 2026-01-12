#pragma once
#include <string>
#include <vector>
#include "PostList.h"
#include "NotificationQueue.h"
#include "FileManager.h"




class User {
public:
    int userID;
    std::string username;
    std::string password;
    std::string bio;

    std::vector<int> followers;
    std::vector<int> following;

    // BST pointers
    User* left;
    User* right;

    User(int id, const std::string& uname, const std::string& pass);

    // Profile actions
    bool follow(int targetUserID);
    bool unfollow(int targetUserID);
    bool isFollowing(int userID) const;
    PostList posts;
    NotificationQueue notifications;


    void setBio(const std::string& newBio);
};
