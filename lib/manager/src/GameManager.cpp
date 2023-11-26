#include "GameManager.hpp"

#include <iostream>
#include <fstream>
#include <string>

#include <cpp-tree-sitter.h>

extern "C" {
    TSLanguage* tree_sitter_socialgaming();
}

// Define a function to load the contents of a file into a string
//incorporate later?
void loadFile(std::string &contents, const std::string &filename) {
    std::ifstream ifs(filename);
    contents.assign(
        std::istreambuf_iterator<char>(ifs),
        std::istreambuf_iterator<char>()
    );
}

// Constructor for the GameManager class
GameManager::GameManager(std::string_view name, Connection& conn, GeneralManager* generalManager) :
    generalManager(generalManager),
    gameName(name),
    audience(false),
    playerRange(std::make_pair(2, 8)) { //temp playerRange value
    // Initialize the GameManager with the owner and game name
    // Set the initial conditions
    // Adjust playerRange as needed

    // the player's default username is "User <connectionID>"
    auto username = "User " + conn.id;
    addPlayer(username, conn);
}

// Add a player with the given name
// bool GameManager::addPlayer(std::string name) {
//     if (playerMap->size() >= playerRange.first && playerMap->size() < playerRange.second) {
//         // Check if the number of players is within the allowed range
//         auto result = playerMap->emplace(name, User(name, playerMap->size() + 1));
//         return result.second; // Return true if the insertion was successful
//     }
//     return false; // Return false if the player limit has been reached
// }

bool GameManager::addPlayer(std::string name, Connection& conn) {

    if (playerMap->size() >= playerRange.first && playerMap->size() < playerRange.second) {
        // Check if the number of players is within the allowed range

        // add a mapping of a new User and their Connection to GameManager.
        auto result = playerMap->emplace(User(name), conn);
        return result.second; // Return true if the insertion was successful
    }
    return false; // Return false if the player limit has been reached
}

// Remove a player with the given name. Returns true if removed successfully
bool GameManager::removePlayer(std::string name) {
    
    auto it = std::find_if(playerMap.get()->begin(), playerMap.get()->end(),
                           [name](const std::pair<const User, Connection>& pair) {
                               return pair.first.getName() == name;
                           });

    // Check if the User object was found
    if (it != playerMap->end()) {
        // Remove the User-Connection pair from the map
        playerMap->erase(it);
        return true;
    } 
    return false;
}

// Check if a player with the given name exists
// TODO - use this same function in removePlayer()
bool GameManager::hasPlayer(std::string name) {
    
    auto it = std::find_if(playerMap.get()->begin(), playerMap.get()->end(),
                           [name](const std::pair<const User, Connection>& pair) {
                               return pair.first.getName() == name;
                           });

    // Check if the User object was found
    if (it != playerMap->end()) {
        // Remove the User-Connection pair from the map
        return true;
    } 
    return false;
}

bool GameManager::hasConnection(const Connection& conn) {
    
    auto it = std::find_if(playerMap.get()->begin(), playerMap.get()->end(),
                           [conn](const std::pair<const User, Connection>& pair) {
                               return pair.second == conn;
                           });

    // Check if the User object was found
    if (it != playerMap->end()) {
        // Remove the User-Connection pair from the map
        return true;
    } 
    return false;
}

// returns all the connections in the game
std::vector<Connection> GameManager::getConnections() {
    std::vector<Connection> connections;

    for (const auto& conn : playerMap){
        connections.push_back(conn.second);
    }
    return connections;
}

std::string_view GameManager::getGameName() const {
    return gameName;
};

std::vector<Setup> GameManager::getSetups() {
    return setups;
}

// std::map<std::string, ElementPtr> GameManager::getConstants() {
//     return constants;
// }

// std::map<std::string, ElementPtr> GameManager::getVariables() {
//     return variables;
// }

bool ends_with(const std::string_view &str, const std::string_view &end) {
    if (end.size() > str.size()) {
        return false;
    } else {
        return str.substr(str.size() - end.size(), end.size()) == end;
    }
}

void GameManager::parseAndSetRules() {

    // Create a language and parser using the Tree-sitter socialgaming language
    ts::Language language = tree_sitter_socialgaming();
    ts::Parser parser{language};

    std::string rulesTxt = "";
    std::string filename = "test.txt"; // get user input for this eventually

    if (ends_with(filename, ".txt")) {
        loadFile(rulesTxt, "test.txt");
    } else {
        //not a .txt file, can't be parsed. do nothing.
        std::cerr << "please enter a valid .txt file." << std::endl;
        return;
    }

    ts::Tree tree = parser.parseString(rulesTxt);
    ts::Node root = tree.getRootNode();

    //TODO: parse the nodes in the created tree into the rules vector
    //in the gamemanager object
    //do we need a submodule for the tree sitter as well? or is it fine

    auto treestring = root.getSExpr();
    printf("Syntax tree: %s\n", treestring.get());
}
