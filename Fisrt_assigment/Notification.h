#pragma once
#include <string>
#include <ctime>

enum class NotificationType {
    COMMENT = 1,
    LIKE = 2,
    FOLLOW = 3
};

struct Notification {
    NotificationType type;
    std::string message;
    std::time_t timestamp;
    bool isRead;

    Notification(NotificationType t, const std::string& msg)
        : type(t), message(msg),
        timestamp(std::time(nullptr)),
        isRead(false) {
    }
};
