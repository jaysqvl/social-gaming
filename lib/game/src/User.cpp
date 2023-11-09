#include "User.hpp"
#include <string>

User::User(std::string name, uint32_t id) : name(name), id(id){};

std::string User::getName() const {
    return name;
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

bool User::operator==(const User& other) const {
    // Compare the members of the User object to determine equality
    return this->id == other.id; // or this->name == other.name
}