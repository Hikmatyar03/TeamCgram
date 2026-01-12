
#pragma once
#include "User.h"
#include "Encryption.h"
#include <map>
#include <memory>

class UserManager {
    std::map<std::string, User> users;
    User* current = nullptr;
public:
    bool registerUser(const std::string&, const std::string&);
    bool login(const std::string&, const std::string&);
    void logout();
    User* currentUser();
};
