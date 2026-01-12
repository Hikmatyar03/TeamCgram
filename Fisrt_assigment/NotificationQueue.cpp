#include "NotificationQueue.h"

void NotificationQueue::add(NotificationType type, const std::string& message) {
    queue.emplace(type, message);
}

bool NotificationQueue::empty() const {
    return queue.empty();
}

Notification NotificationQueue::top() const {
    return queue.top();
}

void NotificationQueue::pop() {
    if (!queue.empty())
        queue.pop();
}

std::vector<Notification> NotificationQueue::getAllOrdered() {
    std::vector<Notification> result;
    auto temp = queue;

    while (!temp.empty()) {
        result.push_back(temp.top());
        temp.pop();
    }
    return result;
}

void NotificationQueue::markAllRead() {
    std::vector<Notification> temp;

    while (!queue.empty()) {
        Notification n = queue.top();
        n.isRead = true;
        temp.push_back(n);
        queue.pop();
    }

    for (auto& n : temp)
        queue.push(n);
}

void NotificationQueue::clear() {
    while (!queue.empty())
        queue.pop();
}
