
#include "UserManager.h"

bool UserManager::registerUser(const std::string& u, const std::string& p){
    if(users.count(u)) return false;
    User usr;
    usr.username = u;
    usr.passwordHash = Encryption::hashPassword(p);
    usr.id = u;
    users[u] = usr;
    return true;
}
bool UserManager::login(const std::string& u, const std::string& p){
    if(!users.count(u)) return false;
    if(!Encryption::verify(p, users[u].passwordHash)) return false;
    current = &users[u];
    return true;
}
void UserManager::logout(){ current = nullptr; }
User* UserManager::currentUser(){ return current; }
