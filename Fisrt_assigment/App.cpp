#include "App.h"
#include "LoginScreen.h"
#include <algorithm>
#include <iostream>
#include "User.h"
#include "UserBST.h"
App::App()
    : currentUser(nullptr),
    currentScreen(nullptr) {
}

App::~App() {
    if (currentScreen) {
        delete currentScreen;
    }
}

bool App::login(const std::string& username, const std::string& password) {
    User* user = users.getUserByUsername(username);
    if (user && user->password == password) {
        currentUser = user;
        return true;
    }
    return false;
}

bool App::registerUser(const std::string& username, const std::string& password) {
    if (users.getUserByUsername(username)) {
        return false;
    }

    User* newUser = users.registerUser(username, password);
    if (newUser) {
        currentUser = newUser;
        return true;
    }
    return false;
}

void App::logout() {
    currentUser = nullptr;
}

void App::setScreen(Screen* screen) {
    if (currentScreen) {
        delete currentScreen;
    }
    currentScreen = screen;
}

std::vector<Post*> App::getAllPosts() const {
    std::vector<Post*> allPosts;

    for (User* user : users.getAllUsers()) {
        std::vector<Post*> userPosts = user->posts.getAllPosts();
        allPosts.insert(allPosts.end(), userPosts.begin(), userPosts.end());
    }

    return allPosts;
}

Post* App::createPost(int userID, const std::string& content) {
    User* user = users.getUserByID(userID);
    if (!user) return nullptr;

    return user->posts.createPost(userID, content);
}

void App::deletePost(int postID) {
    for (User* user : users.getAllUsers()) {
        user->posts.deletePost(postID);
    }
}

void App::followUser(int followerID, int followeeID) {
    User* follower = users.getUserByID(followerID);
    User* followee = users.getUserByID(followeeID);

    if (!follower || !followee || followerID == followeeID) return;

    auto& following = follower->following;
    if (std::find(following.begin(), following.end(), followeeID) != following.end()) {
        return;
    }

    following.push_back(followeeID);

    std::string message = follower->username + " started following you";
    notificationManager.addNotification(followeeID, followerID,
        NotificationType::FOLLOW, message);
}

void App::unfollowUser(int followerID, int followeeID) {
    User* follower = users.getUserByID(followerID);
    if (!follower) return;

    auto& following = follower->following;
    following.erase(
        std::remove(following.begin(), following.end(), followeeID),
        following.end()
    );
}

void App::likePost(int userID, int postID) {
    Post* post = nullptr;
    User* postAuthor = nullptr;

    for (User* user : users.getAllUsers()) {
        for (Post* p : user->posts.getAllPosts()) {
            if (p->postID == postID) {
                post = p;
                postAuthor = user;
                break;
            }
        }
        if (post) break;
    }

    if (!post || !postAuthor) return;

    if (std::find(post->likedBy.begin(), post->likedBy.end(), userID) != post->likedBy.end()) {
        return;
    }

    post->likedBy.push_back(userID);
    post->likes++;

    if (postAuthor->userID != userID) {
        User* liker = users.getUserByID(userID);
        if (liker) {
            std::string message = liker->username + " liked your post";
            notificationManager.addNotification(postAuthor->userID, userID,
                NotificationType::LIKE, message, postID);
        }
    }
}

void App::unlikePost(int userID, int postID) {
    for (User* user : users.getAllUsers()) {
        for (Post* p : user->posts.getAllPosts()) {
            if (p->postID == postID) {
                p->likedBy.erase(
                    std::remove(p->likedBy.begin(), p->likedBy.end(), userID),
                    p->likedBy.end()
                );
                if (p->likes > 0) p->likes--;
                return;
            }
        }
    }
}

void App::addComment(int postID, int userID, const std::string& content) {
    Post* post = nullptr;
    User* postAuthor = nullptr;

    for (User* user : users.getAllUsers()) {
        for (Post* p : user->posts.getAllPosts()) {
            if (p->postID == postID) {
                post = p;
                postAuthor = user;
                break;
            }
        }
        if (post) break;
    }

    if (!post || !postAuthor) return;

    Comment* newComment = new Comment();
    newComment->userID = userID;
    newComment->text = content;
    newComment->timestamp = std::time(nullptr);
    newComment->next = nullptr;

    if (!post->comments) {
        post->comments = newComment;
    }
    else {
        Comment* current = post->comments;
        while (current->next) {
            current = current->next;
        }
        current->next = newComment;
    }

    if (postAuthor->userID != userID) {
        User* commenter = users.getUserByID(userID);
        if (commenter) {
            std::string message = commenter->username + " commented on your post";
            notificationManager.addNotification(postAuthor->userID, userID,
                NotificationType::COMMENT, message, postID);
        }
    }
}