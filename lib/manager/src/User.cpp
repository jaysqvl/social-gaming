#include "User.hpp"
#include <string>

User::User(std::string name, uint32_t id) : name(name), id(id){};

std::string User::getName() {
    return name;
}

std::map<std::string, ElementPtr> User::getMap() {
    return userMap;
}

bool User::isOwner() {
    return isOwner;
}