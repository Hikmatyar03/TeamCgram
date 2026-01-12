#include "FeedManager.h"
#include "UserBST.h"
#include "User.h"
#include "Post.h"
#include <algorithm>

std::vector<Post*> FeedManager::generateFeed(User* user, const UserBST& users) const
{
    std::vector<Post*> feed;
    if (!user) return feed;

    // own posts
    for (Post* p : user->posts.getAllPosts())
        feed.push_back(p);

    // followed users
    for (int id : user->following)
    {
        User* u = users.getUserByID(id);
        if (!u) continue;

        for (Post* p : u->posts.getAllPosts())
            feed.push_back(p);
    }

    std::sort(feed.begin(), feed.end(),
        [](Post* a, Post* b)
        {
            return a->timestamp > b->timestamp;
        });

    return feed;
}
