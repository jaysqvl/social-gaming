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

    // Game game;
    std::string gameName;
    bool audience;
    std::pair<int, int> playerRange;
    
    // TODO - add back in.
    // User owner

    // // Stores the setups the game has the parse and execute
    std::vector<Setup> setups;

    // //TODO: I think this is all removable.
    // std::vector<RulePtr> rules; // game manager should have rule, but rule should not have gamemanager
    // std::map<std::string, ElementPtr> constants;
    // std::map<std::string, ElementPtr> variables;

    // this contains all system messages that are to be collected by general manager.
    std::vector<Packet> systemMessages;
    
    // Maps connection to User
    // Each "user" has a connection. Hence, the map will be user to connection.
    std::shared_ptr<std::map<User, Connection>> playerMap = std::make_shared<std::map<User, Connection>>();

public:
    
    GameManager(std::string_view name, Connection& conn);
    bool addPlayer(Connection& conn);
    bool removePlayer(std::string name);
    bool hasPlayer(std::string name);
    bool hasConnection(const Connection& conn); 

    std::string getGameName() const;
    std::string getUsername(const Connection& conn);

    std::vector<Setup> getSetups();
    std::vector<Connection> getConnections();
    std::vector<std::pair<Connection, std::string>> getConnectionsUsernames();

    // TODO - REMOVE
    // std::map<std::string, ElementPtr> getConstants();
    // std::map<std::string, ElementPtr> getVariables();
    //std::vector<RulePtr> getRules() { return rules; }

    void sendSystemMessage(std::string message);
    // returns the queued systemMessages, and deletes everything currently there.
    std::vector<Packet> retrieveSystemMessages();
    
    void parseAndSetRules();
};
