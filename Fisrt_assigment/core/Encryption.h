
#pragma once
#include <string>
#include <algorithm>

struct Encryption {
    static std::string hashPassword(const std::string& p) {
        std::string s = p;
        for(char& c: s) c ^= 'K';
        return s;
    }
    static bool verify(const std::string& p, const std::string& h) {
        return hashPassword(p) == h;
    }
};
