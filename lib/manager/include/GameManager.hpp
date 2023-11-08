#pragma once


//none of these files should be circularly depending on this EXCEPT for GeneralManager.
#include "Server.h"
#include "Setup.hpp"
//#include "Rule.hpp"
//#include "Element.hpp"

#include "Game.hpp"
#include "User.hpp"

#include <vector>
#include <string>
#include <map>

using networking::Server;
using networking::Connection;

// class Rule;

class GameManager {
private:
    //using RulePtr = std::shared_ptr<Rule>;

    // TODO - add a vector of User
    // User = {connection, room}
    // TODO - add single reference to Game
    // TODO - remove the below
    

    std::vector<Connection> clients;
    std::vector<User> players;
    std::vector<User> spectators;

    User owner;
    std::string gameName;
    bool audience;
    // Stores the setups the game has the parse and execute
    std::vector<Setup> setups;

public:
    GameManager(std::string name, User owner);
    bool addPlayer(std::string name);
    bool removePlayer(std::string name);
    bool addSpectator(std::string name);
    bool removeSpectator(std::string name);
    bool hasPlayer(std::string name);
    bool hasAudience();

    std::vector<Setup> getSetups();
    // std::map<std::string, ElementPtr> getConstants();
    // std::map<std::string, ElementPtr> getVariables();

    //std::vector<RulePtr> getRules() { return rules; }
    void parseAndSetRules();

};
