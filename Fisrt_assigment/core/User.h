
#pragma once
#include <string>
#include <vector>

struct User {
    std::string id;
    std::string username;
    std::string passwordHash;
    std::string bio;
    std::vector<std::string> followers;
    std::vector<std::string> following;
};
