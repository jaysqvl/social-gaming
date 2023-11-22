#pragma once


//none of these files should be circularly depending on this EXCEPT for GeneralManager.
#include "Server.h"
#include "Setup.hpp"
#include "User.hpp"
//#include "Rule.hpp"
//#include "Element.hpp"

#include "GeneralManager.hpp"

#include "Game.hpp"

#include <vector>
#include <string>
#include <map>

using networking::Server;
using networking::Connection;

// class Rule;

// Resolve circular dependency
class GeneralManager;

class GameManager {
private:
    //using RulePtr = std::shared_ptr<Rule>;

    // using this, you can send commands back to the parent general manager.
    GeneralManager* generalManager;

    std::vector<Connection> clients;
    // Game game;
    std::string_view gameName;
    bool audience;
    std::pair<int, int> playerRange;


    // User owner;
    // // Stores the setups the game has the parse and execute
    std::vector<Setup> setups;

    // //TODO: FIX CIRCULAR DEPENDENCY
    // std::vector<RulePtr> rules; // game manager should have rule, but rule should not have gamemanager

    // std::map<std::string, ElementPtr> constants;
    // std::map<std::string, ElementPtr> variables;

    // //A map holding key value pairs to store a unique user's name, as well as the data for that user

    // TODO - fix / change this.
    std::shared_ptr<std::map<std::string, User>> playerMap = std::make_shared<std::map<std::string, User>>();
    std::shared_ptr<std::map<std::string, User>> spectatorMap = std::make_shared<std::map<std::string, User>>();

public:
    
    // TODO - change back to users maybe?

    GameManager(std::string_view name, Connection& conn, GeneralManager* generalManager);
    // GameManager(std::string name, User owner, std::vector<Connection> clients);
    bool addPlayer(std::string name);
    bool removePlayer(std::string name);
    bool addSpectator(std::string name);
    bool removeSpectator(std::string name);
    bool hasPlayer(std::string name);
    bool hasAudience();

    // function that sends message to a list of players in the game


    std::vector<Setup> getSetups();
    // std::map<std::string, ElementPtr> getConstants();
    // std::map<std::string, ElementPtr> getVariables();

    //std::vector<RulePtr> getRules() { return rules; }
    void parseAndSetRules();

};
