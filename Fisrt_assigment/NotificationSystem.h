#pragma once
#include <string>
#include <ctime>
#include <vector>

enum class NotificationType {
    FOLLOW,
    LIKE,
    COMMENT,
    SYSTEM
};

struct Notification {
    int notificationID;
    int userID;           // Who receives the notification
    int fromUserID;       // Who triggered it
    NotificationType type;
    std::string message;
    time_t timestamp;
    bool isRead;
    int relatedPostID;    // For likes/comments

    Notification* next;

    Notification(int uid, int fuid, NotificationType t, const std::string& msg, int postID = -1)
        : notificationID(0), userID(uid), fromUserID(fuid), type(t),
        message(msg), timestamp(std::time(nullptr)), isRead(false),
        relatedPostID(postID), next(nullptr) {
    }
};

class NotificationManager {
private:
    Notification* head;
    int nextID;

public:
    NotificationManager() : head(nullptr), nextID(1) {}

    ~NotificationManager() {
        while (head) {
            Notification* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void addNotification(int userID, int fromUserID, NotificationType type,
        const std::string& message, int postID = -1) {
        Notification* notif = new Notification(userID, fromUserID, type, message, postID);
        notif->notificationID = nextID++;
        notif->next = head;
        head = notif;
    }

    std::vector<Notification*> getUnreadNotifications(int userID) const {
        std::vector<Notification*> unread;
        for (Notification* n = head; n != nullptr; n = n->next) {
            if (n->userID == userID && !n->isRead) {
                unread.push_back(n);
            }
        }
        return unread;
    }

    std::vector<Notification*> getAllNotifications(int userID) const {
        std::vector<Notification*> all;
        for (Notification* n = head; n != nullptr; n = n->next) {
            if (n->userID == userID) {
                all.push_back(n);
            }
        }
        return all;
    }

    void markAsRead(int notificationID) {
        for (Notification* n = head; n != nullptr; n = n->next) {
            if (n->notificationID == notificationID) {
                n->isRead = true;
                break;
            }
        }
    }

    void markAllAsRead(int userID) {
        for (Notification* n = head; n != nullptr; n = n->next) {
            if (n->userID == userID) {
                n->isRead = true;
            }
        }
    }

    int getUnreadCount(int userID) const {
        int count = 0;
        for (Notification* n = head; n != nullptr; n = n->next) {
            if (n->userID == userID && !n->isRead) {
                count++;
            }
        }
        return count;
    }
};