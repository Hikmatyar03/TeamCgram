#pragma once
#include <queue>
#include <vector>
#include "Notification.h"

class NotificationQueue {
private:
    struct Compare {
        bool operator()(const Notification& a, const Notification& b) const {
            if (static_cast<int>(a.type) == static_cast<int>(b.type))
                return a.timestamp > b.timestamp; // older first
            return static_cast<int>(a.type) > static_cast<int>(b.type);
        }
    };

    std::priority_queue<Notification,
        std::vector<Notification>,
        Compare> queue;

public:
    void add(NotificationType type, const std::string& message);

    bool empty() const;
    Notification top() const;
    void pop();

    std::vector<Notification> getAllOrdered();
    void markAllRead();
    void clear();
};
#pragma once
