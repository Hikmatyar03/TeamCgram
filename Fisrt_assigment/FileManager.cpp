#include "FileManager.h"
#include "UserBST.h"
#include <fstream>
#include <sstream>
#include <algorithm>

#include "User.h"
#include "Post.h"
#include "NotificationSystem.h"  // CHANGE THIS LINE from "Notification.h"

// Remove or comment out the notification save/load functions since
// notifications are now managed centrally in App, not per-user

void FileManager::createUserFile(const std::string& username) {
    std::ofstream out(basePath + "users/" + username + ".txt", std::ios::app);
}

void FileManager::loadConnections(UserBST& users) {
    std::ifstream in(basePath + "connections.txt");
    std::string line;

    while (std::getline(in, line)) {
        std::stringstream ss(line);
        std::string follower, target;

        std::getline(ss, follower, ',');
        std::getline(ss, target);

        users.followUser(std::stoi(follower), std::stoi(target));
    }
}

void FileManager::loadDummyFeed(UserBST& users) {
    std::ifstream in(basePath + "demo_feed.txt");
    if (!in) return;

    std::string line;
    while (std::getline(in, line)) {
        std::stringstream ss(line);
        std::string username, content;

        std::getline(ss, username, '|');
        std::getline(ss, content);

        User* u = users.getUserByUsername(username);
        if (!u) continue;

        u->posts.createPost(u->userID, content);
    }
}

void FileManager::appendToGlobalFeed(const User& user, const Post& post) {
    std::ofstream out(basePath + "feed.txt", std::ios::app);
    out << "POST|" << post.postID << "|" << user.username << "|" << post.timestamp << "\n";
}

// COMMENT OUT OR REMOVE these notification functions
// since User no longer has notifications member
/*
void FileManager::saveNotifications(const UserBST& users) const {
    // Remove - notifications are now in App
}

void FileManager::loadNotifications(UserBST& users) {
    // Remove - notifications are now in App
}
*/

void FileManager::loadUserData(User& user) {
    std::ifstream in(basePath + "users/" + user.username + ".txt");
    if (!in) return;

    std::string line;
    while (std::getline(in, line)) {
        if (line.rfind("BIO|", 0) == 0)
            user.bio = line.substr(4);
    }
}

void FileManager::saveUserData(const User& user) {
    std::ofstream out(basePath + "users/" + user.username + ".txt");

    out << "ID|" << user.userID << "\n";
    out << "BIO|" << user.bio << "\n";

    for (Post* p : user.posts.getAllPosts()) {
        out << "POST|" << p->postID << "|" << p->content << "|" << p->timestamp << "|" << p->likes << "\n";

        for (Comment* c = p->comments; c; c = c->next)
            out << "COMMENT|" << c->userID << "|" << c->text << "\n";

        out << "ENDPOST\n";
    }
}

FileManager::FileManager(const std::string& path) {
    this->basePath = path;
    if (!basePath.empty() && basePath.back() != '/' && basePath.back() != '\\') {
        basePath += "/";
    }
}

void FileManager::loadUsers(UserBST& users) {
    std::ifstream in(basePath + "users.txt");
    if (!in) return;

    std::string line;
    while (std::getline(in, line)) {
        std::stringstream ss(line);
        std::string id, username, password;

        std::getline(ss, id, '|');
        std::getline(ss, username, '|');
        std::getline(ss, password);

        users.registerUser(username, password);
    }
}

void FileManager::loadPosts(UserBST& users) {
    std::ifstream in(basePath + "posts.txt");
    if (!in) return;

    std::string line;
    while (std::getline(in, line)) {
        std::stringstream ss(line);
        std::string postID, userID, content, timestamp, likes;

        std::getline(ss, postID, '|');
        std::getline(ss, userID, '|');
        std::getline(ss, content, '|');
        std::getline(ss, timestamp, '|');
        std::getline(ss, likes);

        User* u = users.getUserByID(std::stoi(userID));
        if (!u) continue;

        Post* p = u->posts.createPost(std::stoi(userID), content);
        p->postID = std::stoi(postID);
        p->timestamp = std::stol(timestamp);
        p->likes = std::stoi(likes);
    }
}

void FileManager::createDefaultPosts(User& user) {
    std::vector<std::string> defaultPosts = {
        "Hey everyone! " + user.username + " here. Excited to be part of this community! 🎉",
        "Just created my account. Looking forward to sharing and connecting with you all!",
        "New here! Drop a comment and let's connect! 👋"
    };

    for (const auto& postContent : defaultPosts) {
        Post* p = user.posts.createPost(user.userID, postContent);
        appendToGlobalFeed(user, *p);
    }
}

void FileManager::saveConnections(const UserBST& users) const {
    std::ofstream out(basePath + "connections.txt");
    std::vector<User*> allUsers = users.getAllUsers();
    for (User* u : allUsers) {
        for (int target : u->following) {
            out << u->userID << "," << target << "\n";
        }
    }
}

void FileManager::savePosts(const UserBST& users) const {
    std::ofstream out(basePath + "posts.txt");
    std::vector<User*> allUsers = users.getAllUsers();
    for (User* u : allUsers) {
        std::vector<Post*> userPosts = u->posts.getAllPosts();
        for (Post* p : userPosts) {
            out << p->postID << "|" << p->userID << "|" << p->content << "|"
                << p->timestamp << "|" << p->likes << "\n";
        }
    }
}

void FileManager::saveUsers(const UserBST& users) const {
    std::ofstream out(basePath + "users.txt");
    std::vector<User*> allUsers = users.getAllUsers();
    for (User* u : allUsers) {
        out << u->userID << "|" << u->username << "|" << u->password << "\n";
    }
}