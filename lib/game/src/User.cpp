#include "User.hpp"
#include <string>

// User::User(std::string name, uint32_t id) : name(name), id(id){};

// TODO - fix back to normal ID
User::User(uint32_t id) : id(id), name("User " + std::to_string(id)) {};

std::string User::getName() const {
    return name;
}

bool User::operator<(const User& other) const {
    return id < other.getId();
}

uint32_t User::getId() const {
    return id;
}

// std::map<std::string, ElementPtr> User::getMap() const {
//     return userMap;
// }

void User::setRole(UserRole newRole) {
    role = newRole;
}

UserRole User::getRole() const {
    return role;
}