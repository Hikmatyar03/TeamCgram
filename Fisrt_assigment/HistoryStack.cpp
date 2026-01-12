#include "HistoryStack.h"

void HistoryStack::pushProfile(int userID) {
    history.emplace(HistoryType::PROFILE, userID);
}

void HistoryStack::pushPost(int postID) {
    history.emplace(HistoryType::POST, postID);
}

bool HistoryStack::empty() const {
    return history.empty();
}

HistoryItem HistoryStack::top() const {
    return history.top();
}

HistoryItem HistoryStack::pop() {
    HistoryItem item = history.top();
    history.pop();
    return item;
}

std::vector<HistoryItem> HistoryStack::getAll() const {
    std::vector<HistoryItem> items;
    std::stack<HistoryItem> temp = history;

    while (!temp.empty()) {
        items.push_back(temp.top());
        temp.pop();
    }

    return items;
}

void HistoryStack::clear() {
    while (!history.empty())
        history.pop();
}
