#include "User.hpp"
#include <string>

User::User(std::string name, uint32_t id) : name(name), id(id){};

std::string User::getName() const {
    return name;
}

uint32_t User::getId() const {
    return id;
}

std::map<std::string, ElementPtr> User::getMap() const {
    return userMap;
}

void User::setRole(UserRole newRole) {
    role = newRole;
}

UserRole User::getRole() {
    return role;
}