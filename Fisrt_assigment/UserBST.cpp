#include "UserBST.h"
#include <algorithm>

// ==========================
// CONSTRUCTOR / DESTRUCTOR
// ==========================

UserBST::UserBST()
    : root(nullptr), nextUserID(1) {
}

UserBST::~UserBST()
{
    // Delete all users (nodes will be deleted recursively)
    std::vector<User*> users = getAllUsers();
    for (User* u : users)
        delete u;
}

// ==========================
// INTERNAL BST HELPERS
// ==========================

UserNode* UserBST::insert(UserNode* node, User* newUser)
{
    if (!node)
        return new UserNode(newUser);

    if (newUser->userID < node->user->userID)
        node->left = insert(node->left, newUser);
    else
        node->right = insert(node->right, newUser);

    return node;
}


bool UserBST::registerLoadedUser(
    int id,
    const std::string& username,
    const std::string& password,
    const std::string& bio
)
{
    // Prevent duplicate usernames
    if (!searchUserByUsername(username).empty())
        return false;

    User* u = new User(id, username, password);
    u->bio = bio;

    root = insert(root, u);

    // Keep IDs consistent
    if (id >= nextUserID)
        nextUserID = id + 1;

    return true;
}


UserNode* UserBST::searchByID(UserNode* node, int userID) const
{
    if (!node)
        return nullptr;

    if (userID == node->user->userID)
        return node;

    if (userID < node->user->userID)
        return searchByID(node->left, userID);

    return searchByID(node->right, userID);
}

void UserBST::searchByUsername(
    UserNode* node,
    const std::string& username,
    std::vector<User*>& results
) const
{
    if (!node)
        return;

    if (node->user->username == username)
        results.push_back(node->user);

    searchByUsername(node->left, username, results);
    searchByUsername(node->right, username, results);
}

void UserBST::inorder(UserNode* node, std::vector<User*>& users) const
{
    if (!node)
        return;

    inorder(node->left, users);
    users.push_back(node->user);
    inorder(node->right, users);
}

// ==========================
// PUBLIC API
// ==========================

bool UserBST::registerUser(const std::string& username, const std::string& password)
{
    if (password.length() < 6)
        return false;

    // Enforce unique username
    if (!searchUserByUsername(username).empty())
        return false;

    User* newUser = new User(nextUserID++, username, password);
    root = insert(root, newUser);
    return true;
}

User* UserBST::login(const std::string& username, const std::string& password)
{
    auto matches = searchUserByUsername(username);
    if (matches.empty())
        return nullptr;

    User* user = matches.front();
    if (user->password != password)
        return nullptr;

    return user;
}

User* UserBST::getUserByID(int id) const
{
    UserNode* node = searchByID(root, id);
    return node ? node->user : nullptr;
}

User* UserBST::getUserByUsername(const std::string& username)
{
    auto v = searchUserByUsername(username);
    return v.empty() ? nullptr : v.front();
}

std::vector<User*> UserBST::searchUserByUsername(const std::string& username) const
{
    std::vector<User*> results;

    searchByUsername(root, username, results);
    return results;
}

std::vector<User*> UserBST::getAllUsers() const
{
    std::vector<User*> users;
    inorder(root, users);
    return users;
}

// ==========================
// FOLLOW / UNFOLLOW
// ==========================

bool UserBST::followUser(int followerID, int targetID)
{
    if (followerID == targetID)
        return false;

    User* follower = getUserByID(followerID);
    User* target = getUserByID(targetID);

    if (!follower || !target)
        return false;

    if (!follower->follow(targetID))
        return false;

    target->followers.push_back(followerID);
    return true;
}

bool UserBST::unfollowUser(int followerID, int targetID)
{
    User* follower = getUserByID(followerID);
    User* target = getUserByID(targetID);

    if (!follower || !target)
        return false;

    if (!follower->unfollow(targetID))
        return false;

    auto& f = target->followers;
    f.erase(std::remove(f.begin(), f.end(), followerID), f.end());
    return true;
}
