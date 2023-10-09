#include "Spectator.hpp"

#include <string>

Spectator::Spectator(std::string name) : Player(name){};

std::string Spectator::getName() {
    return name;
}

std::map<std::string, std::vector<std::variant<int, double, std::string, bool>>> Spectator::getMap() {
    return spectatorMap;
}

std::vector<std::variant<int, double, std::string, bool>> Spectator::getList() {
    return spectatorList;
}
