
#pragma once
#include <string>
#include <ctime>

struct Post {
    std::string id;
    std::string user;
    std::string content;
    std::time_t timestamp;
    int likes;
};
