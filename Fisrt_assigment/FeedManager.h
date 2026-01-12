#pragma once
#include <vector>

class User;
class UserBST;
class Post;

class FeedManager
{
public:
    std::vector<Post*> generateFeed(User* user, const UserBST& users) const;
};
