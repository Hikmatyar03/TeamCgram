#pragma once
#include <string>
#include <ctime>

struct Comment {
    int userID;
    std::string text;
    std::time_t timestamp;
    Comment* next;

    Comment(int uid, const std::string& t)
        : userID(uid), text(t), timestamp(std::time(nullptr)), next(nullptr) {
    }
};
