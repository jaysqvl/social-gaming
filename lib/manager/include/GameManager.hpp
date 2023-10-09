#pragma once

#include "Server.h"
#include "Setup.hpp"
#include "Player.hpp"
#include "Spectator.hpp"
#include "Rule.hpp"

#include <vector>
#include <string>
#include <map>

using networking::Server;
using networking::Connection;

class GameManager {
private:
    std::vector<Connection> clients;

    std::string name;
    std::pair<int, int> playerRange;
    bool hasAudience;
    // Stores the setups the game has the parse and execute
    std::vector<Setup> setups;
    std::vector<Rule> rules;
    std::map<std::string, std::string> constants;
    std::map<std::string, std::string> variables;
    std::map<std::string, Player> playerMaps;
    std::map<std::string, Spectator> spectatorMap;

public:
    GameManager(std::string name, std::pair<int, int> playerRange, bool hasAudience, std::vector<Setup> setups);
};