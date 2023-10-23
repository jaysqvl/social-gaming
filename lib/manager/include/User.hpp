#pragma once

#include "Element.hpp"

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

    //TODO: maybe look into element and see if there is circular dependency
    std::map<std::string, ElementPtr> userMap;
    ElementPtr element;
    UserRole role = UserRole::Audience; // Default: Audience

public:
    User(std::string name, uint32_t id);
    std::string getName() const;
    uint32_t getId() const;
    std::map<std::string, ElementPtr> getMap() const;
    void setRole(UserRole newRole);
    UserRole getRole() const;
};