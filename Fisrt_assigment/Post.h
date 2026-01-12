#pragma once
#include <string>
#include <vector>
#include <ctime>

struct Comment
{
    int userID;
    std::string text;
    std::time_t timestamp;   // ✅ REQUIRED
    Comment* next;

    Comment(int uid, const std::string& t)
        : userID(uid),
        text(t),
        timestamp(std::time(nullptr)), // ✅ INITIALIZED
        next(nullptr)
    {
    }
};


class Post
{
public:
    int postID;
    int userID;
    std::string content;
    std::time_t timestamp;
    int likes;

    Comment* comments;
    Post* prev;
    Post* next;

    std::vector<int> likedBy;   // prevents double-like

    // ✅ FULLY INITIALIZED CONSTRUCTOR
    Post(int pid, int uid, const std::string& content);

    // Actions
    bool like(int uid);
    bool unlike(int uid);
    void addComment(int uid, const std::string& text);
};
