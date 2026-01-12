#include "PostList.h"
#include "Post.h"

PostList::PostList()
    : head(nullptr),
    tail(nullptr),
    nextPostID(1)
{
}

PostList::~PostList()
{
    Post* curr = head;
    while (curr)
    {
        Post* next = curr->next;

        // Delete comments
        Comment* c = curr->comments;
        while (c)
        {
            Comment* tmp = c;
            c = c->next;
            delete tmp;
        }

        delete curr;
        curr = next;
    }
}

Post* PostList::createPost(int userID, const std::string& content)
{
    if (content.empty() || content.length() > 280)
        return nullptr;

    Post* post = new Post(
        nextPostID++,   // postID
        userID,         // owner userID
        content
    );

    if (!head)
    {
        head = tail = post;
    }
    else
    {
        tail->next = post;
        post->prev = tail;
        tail = post;
    }

    return post;
}

bool PostList::deletePost(int postID)
{
    Post* post = getPostByID(postID);
    if (!post)
        return false;

    if (post->prev)
        post->prev->next = post->next;
    else
        head = post->next;

    if (post->next)
        post->next->prev = post->prev;
    else
        tail = post->prev;

    delete post;
    return true;
}

bool PostList::editPost(int postID, const std::string& newContent)
{
    if (newContent.empty() || newContent.length() > 280)
        return false;

    Post* post = getPostByID(postID);
    if (!post)
        return false;

    post->content = newContent;
    return true;
}

Post* PostList::getPostByID(int postID) const
{
    Post* curr = head;
    while (curr)
    {
        if (curr->postID == postID)
            return curr;
        curr = curr->next;
    }
    return nullptr;
}

std::vector<Post*> PostList::getAllPosts() const
{
    std::vector<Post*> posts;
    Post* curr = head;

    while (curr)
    {
        posts.push_back(curr);
        curr = curr->next;
    }

    return posts;
}
