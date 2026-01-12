#pragma once

#include <string>
#include <vector>
#include <ctime>

// =====================
// Comment Node
// =====================

struct Comment {
    int userID;
    std::string text;
    time_t timestamp;
    Comment* next;

    // Add default constructor
    Comment()
        : userID(0), text(""), timestamp(0), next(nullptr) {
    }

    // Optional: parameterized constructor
    Comment(int uid, const std::string& txt)
        : userID(uid), text(txt), timestamp(std::time(nullptr)), next(nullptr) {
    }
};
// =====================
// Post Class
// =====================
class Post {
public:
    int postID;
    int userID;
    std::string content;
    time_t timestamp;

    int likes;                     // REQUIRED
    std::vector<int> likedBy;      // REQUIRED
    Comment* comments;             // REQUIRED

    Post* prev;
    Post* next;

    // Constructor
    Post(int pid, int uid, const std::string& content);

    // Like system
    bool like(int uid);
    bool unlike(int uid);

    // Comments
    void addComment(int uid, const std::string& text);
};
