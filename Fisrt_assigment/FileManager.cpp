#include "FileManager.h"
#include "UserBST.h"   // REQUIRED
#include <fstream>
#include <sstream>
#include <algorithm>

#include "User.h"
#include "Post.h"



void FileManager::saveConnections(const UserBST& users) const{
    std::ofstream out(basePath + "connections.txt");

    for (User* u : users.getAllUsers()) {
        for (int target : u->following) {
            out << u->userID << "," << target << "\n";
        }
    }
}
void FileManager::createUserFile(const std::string& username)
{
    std::ofstream out(basePath + "users/" + username + ".txt", std::ios::app);
}


void FileManager::loadConnections(UserBST& users)  {
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
void FileManager::loadDummyFeed(UserBST& users)
{
    std::ifstream in(basePath + "demo_feed.txt");
    if (!in) return;

    std::string line;
    while (std::getline(in, line))
    {
        // format: username|content
        std::stringstream ss(line);
        std::string username, content;

        std::getline(ss, username, '|');
        std::getline(ss, content);

        User* u = users.getUserByUsername(username);
        if (!u) continue;

        u->posts.createPost(u->userID, content);
    }
}

void FileManager::appendToGlobalFeed(const User& user, const Post& post)
{
    std::ofstream out(basePath + "feed.txt", std::ios::app);

    out << "POST|"
        << post.postID << "|"
        << user.username << "|"
        << post.timestamp << "\n";
}



void FileManager::saveNotifications(const UserBST& users) const {
    std::ofstream out(basePath + "notifications.txt");

    for (User* u : users.getAllUsers()) {
        auto notifs = u->notifications.getAllOrdered();
        for (auto& n : notifs) {
            out << u->userID << "|"
                << static_cast<int>(n.type) << "|"
                << n.timestamp << "|"
                << n.isRead << "|"
                << n.message << "\n";
        }
    }
}
void FileManager::loadNotifications(UserBST& users) {
    std::ifstream in(basePath + "notifications.txt");
    std::string line;

    while (std::getline(in, line)) {
        std::stringstream ss(line);
        std::string uid, type, ts, read, msg;

        std::getline(ss, uid, '|');
        std::getline(ss, type, '|');
        std::getline(ss, ts, '|');
        std::getline(ss, read, '|');
        std::getline(ss, msg);

        User* u = users.getUserByID(std::stoi(uid));
        if (!u) continue;

        Notification n(
            static_cast<NotificationType>(std::stoi(type)),
            msg
        );
        n.timestamp = std::stol(ts);
        n.isRead = (read == "1");

        u->notifications.add(n.type, n.message);
    }
}
void FileManager::loadUserData(User& user)
{
    std::ifstream in(basePath + "users/" + user.username + ".txt");
    if (!in) return;

    std::string line;
    while (std::getline(in, line))
    {
        if (line.rfind("BIO|", 0) == 0)
            user.bio = line.substr(4);
    }
}


void FileManager::saveUserData(const User& user)
{
    std::ofstream out(basePath + "users/" + user.username + ".txt");

    out << "ID|" << user.userID << "\n";
    out << "BIO|" << user.bio << "\n";

    for (Post* p : user.posts.getAllPosts())
    {
        out << "POST|" << p->postID << "|" << p->content << "|" << p->timestamp << "|" << p->likes << "\n";

        for (Comment* c = p->comments; c; c = c->next)
            out << "COMMENT|" << c->userID << "|" << c->text << "\n";

        out << "ENDPOST\n";
    }
}



// Add this constructor implementation
FileManager::FileManager(const std::string& path) {
    this->basePath = path;

    // Optional safety: Ensure path ends with a slash so file names concatenate correctly
    // e.g., if path is "data", make it "data/" so you get "data/users.txt"
    if (!basePath.empty() && basePath.back() != '/' && basePath.back() != '\\') {
        basePath += "/";
    }
}

void FileManager::saveUsers(const UserBST& users) const {
    std::ofstream out(basePath + "users.txt");

    for (User* u : users.getAllUsers()) {
        out << u->userID << "|"
            << u->username << "|"
            << u->password << "\n";
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

void FileManager::savePosts(const UserBST& users) const {
    std::ofstream out(basePath + "posts.txt");

    for (User* u : users.getAllUsers()) {
        for (Post* p : u->posts.getAllPosts()) {
            out << p->postID << "|"
                << p->userID << "|"
                << p->content << "|"
                << p->timestamp << "|"
                << p->likes << "\n";
        }
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