#include "Post.h"
#include <algorithm>

// ✅ Constructor (MATCHES Post.h EXACTLY)
Post::Post(int pid, int uid, const std::string& content)
    : postID(pid),
    userID(uid),
    content(content),
    timestamp(std::time(nullptr)),
    likes(0),
    comments(nullptr),
    prev(nullptr),
    next(nullptr)
{
}

// ✅ Like
bool Post::like(int uid)
{
    if (std::find(likedBy.begin(), likedBy.end(), uid) != likedBy.end())
        return false;

    likedBy.push_back(uid);
    ++likes;
    return true;
}

// ✅ Unlike
bool Post::unlike(int uid)
{
    auto it = std::find(likedBy.begin(), likedBy.end(), uid);
    if (it == likedBy.end())
        return false;

    likedBy.erase(it);
    --likes;
    return true;
}

// ✅ Add comment (MATCHES Post.h EXACTLY)
void Post::addComment(int uid, const std::string& text)
{
    Comment* c = new Comment(uid, text);
    c->next = comments;
    comments = c;
}
