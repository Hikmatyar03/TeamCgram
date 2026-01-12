#include "User.h"
#include <algorithm>

User::User(int id, const std::string& uname, const std::string& pass)
    : userID(id), username(uname), password(pass), bio(""),
    left(nullptr), right(nullptr) {
}

bool User::follow(int targetUserID) {
    if (targetUserID == userID) return false;

    if (std::find(following.begin(), following.end(), targetUserID) != following.end())
        return false;

    following.push_back(targetUserID);
    return true;
}




bool User::unfollow(int targetUserID) {
    auto it = std::find(following.begin(), following.end(), targetUserID);
    if (it == following.end()) return false;

    following.erase(it);
    return true;
}

bool User::isFollowing(int id) const {
    return std::find(following.begin(), following.end(), id) != following.end();
}

void User::setBio(const std::string& newBio) {
    bio = newBio;
}
