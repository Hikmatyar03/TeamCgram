#pragma once
#include <stack>
#include <vector>
#include <ctime>

enum class HistoryType {
    PROFILE,
    POST
};

struct HistoryItem {
    HistoryType type;
    int referenceID;
    std::time_t timestamp;

    HistoryItem(HistoryType t, int id)
        : type(t), referenceID(id), timestamp(std::time(nullptr)) {
    }
};

class HistoryStack {
private:
    std::stack<HistoryItem> history;

public:
    void pushProfile(int userID);
    void pushPost(int postID);

    bool empty() const;
    HistoryItem pop();
    HistoryItem top() const;

    std::vector<HistoryItem> getAll() const;
    void clear();
};
