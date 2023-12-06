#pragma once

// #include "Element.hpp"

#include <stdint.h>
#include <vector>
#include <string>
#include <variant>
#include <map>
#include <cstdint>

enum class UserRole {
    Owner,
    Player,
    Audience
};

class User {
private:
    std::string name;
    uint32_t id;

    UserRole role = UserRole::Audience; // Default: Audience

public:
    User(uint32_t id);
    // User(std::string name, uint32_t id);
    std::string getName() const;
    // bool changeName();
    uint32_t getId() const;
    bool operator<(const User& other) const; 
    // std::map<std::string, ElementPtr> getMap() const;
    void setRole(UserRole newRole);
    UserRole getRole() const;

};