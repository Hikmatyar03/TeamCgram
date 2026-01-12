#pragma once
#include "Post.h"
#include <vector>

class PostList {
private:
    Post* head;
    Post* tail;
    int nextPostID;

public:
    PostList();
    ~PostList();

    Post* createPost(int userID, const std::string& content);
    bool deletePost(int postID);
    bool editPost(int postID, const std::string& newContent);

    Post* getPostByID(int postID) const;
    std::vector<Post*> getAllPosts() const;
};
