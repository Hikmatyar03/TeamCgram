#pragma once
#include <string>
#include <vector>
#include "User.h"

// ==========================
// BST NODE
// ==========================
struct UserNode {
    User* user;
    UserNode* left;
    UserNode* right;

    explicit UserNode(User* u)
        : user(u), left(nullptr), right(nullptr) {
    }
};

// ==========================
// USER BST
// ==========================
class UserBST {
private:
    UserNode* root;
    int nextUserID;

    // Internal helpers (BST-based)
    UserNode* insert(UserNode* node, User* newUser);
    UserNode* searchByID(UserNode* node, int userID) const;
    void inorder(UserNode* node, std::vector<User*>& users) const;
    void searchByUsername(
        UserNode* node,
        const std::string& username,
        std::vector<User*>& results
    ) const;

public:
    UserBST();
    ~UserBST();

    bool registerLoadedUser(
        int id,
        const std::string& username,
        const std::string& password,
        const std::string& bio
    );

    // Core operations - CHANGED: registerUser now returns User*
    User* registerUser(const std::string& username, const std::string& password);
    User* login(const std::string& username, const std::string& password);

    User* getUserByID(int id) const;
    User* getUserByUsername(const std::string& username);

    std::vector<User*> getAllUsers() const;
    std::vector<User*> searchUserByUsername(const std::string& username) const;

    bool followUser(int followerID, int targetID);
    bool unfollowUser(int followerID, int targetID);
};