#include "Player.hpp"

#include <string>

Player::Player(std::string name) : name(name){};

std::string Player::getName() {
    return name;
}

std::map<std::string, std::vector<std::variant<int, double, std::string, bool>>> Player::getMap() {
    return playerMap;
}

std::vector<std::variant<int, double, std::string, bool>> Player::getList() {
    return playerList;
}
