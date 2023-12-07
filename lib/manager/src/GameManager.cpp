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

bool ends_with(const std::string_view &str, const std::string_view &end) {
    if (end.size() > str.size()) {
        return false;
    } else {
        return str.substr(str.size() - end.size(), end.size()) == end;
    }
}

// Constructor for the GameManager class
GameManager::GameManager(std::string_view name, Connection& conn) :
    gameName(name),
    audience(false),
    playerRange(std::make_pair(2, 8)) { // Default playerRange value
    // The player's default username is "User <connectionID>"
    addPlayer(conn);
}


bool GameManager::addPlayer(Connection& conn) {

    // make sure that the player limit has not been reached
    if (playerMap->size() + 1 < playerRange.second) {

        // Check if the number of players is within the allowed range
        std::cout << std::to_string(conn.id) + " added to game " + getGameName() << std::endl;

        auto newUser = User(conn.id);

        // if this is the only player in the current game, they become the owner.
        // if (playerMap->size() == 0){
        //     owner = newUser;
        // }

        // add a mapping of a new User and their Connection to GameManager.
        auto result = playerMap->emplace(newUser, conn);

        // let everyone in the server know that a new player has joined
        sendSystemMessage("Welcome player " + newUser.getName() + " to the game lobby " + getGameName());

        return result.second; // Return true if the insertion was successful
    }
    
    return false; // Return false if the player limit has been reached
}


bool GameManager::changePlayerName(const Connection& conn, const std::string& newName) {

    auto it = std::find_if(playerMap.get()->begin(), playerMap.get()->end(),
                        [conn](const std::pair<const User, Connection>& pair) {
                            return pair.second == conn;
                        });

    // Check if the User object was found
    if (it != playerMap->end()) {
        // Change the username
        // this is sketchy, but it works.
        auto& user = const_cast<User&>(it->first);
        user.setName(newName);
        return true;
    } 
    return false;
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

// check if a given connection exists in this game
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

// Remove a connection. Returns true if removed successfully
bool GameManager::removeConnection(const Connection& conn) {
    auto it = std::find_if(playerMap.get()->begin(), playerMap.get()->end(),
                           [conn](const std::pair<const User, Connection>& pair) {
                               return pair.second == conn;
                           });

    // Check if the User object was found
    if (it != playerMap->end()) {
        // Remove the User-Connection pair from the map
        playerMap->erase(it);
        return true;
    } 
    return false;
}

std::string GameManager::getUsername(const Connection& conn){

    auto it = std::find_if(playerMap.get()->begin(), playerMap.get()->end(),
                           [conn](const std::pair<const User, Connection>& pair) {
                               return pair.second == conn;
                           });

    // Check if the User object was found
    if (it != playerMap->end()) {
        // Remove the User-Connection pair from the map
        return it->first.getName();
    } 
    return "Null";
}

// returns all the connections in the current game
std::vector<Connection> GameManager::getConnections() {
    std::vector<Connection> connections;
    const std::map<User, Connection>& playerMapLocal = *playerMap;

    for (auto it = playerMapLocal.begin(); it != playerMapLocal.end(); ++it) {
        const Connection& conn = it->second;
        connections.push_back(conn);
        // Process user and connection
    }
    return connections;
}

// returns all connections and their usernames in the current game
std::vector<std::pair<Connection, std::string>> GameManager::getConnectionsUsernames(){
    std::vector<std::pair<Connection, std::string>> connections;
    const std::map<User, Connection>& playerMapLocal = *playerMap;

    for (auto it = playerMapLocal.begin(); it != playerMapLocal.end(); ++it) {
        connections.push_back(std::make_pair(it->second, it->first.getName()));
    }
    return connections;
}

std::string GameManager::getGameName() const {
    return gameName;
};

std::vector<Setup> GameManager::getSetups() {
    return setups;
}

void GameManager::sendSystemMessage(std::string message){
    // create a packet to send to each connection in the game.   
    for (const Connection& conn : getConnections()){
            systemMessages.push_back(Packet{PacketType::TO, conn, message});
    }
}

std::vector<Packet> GameManager::retrieveSystemMessages(){
    std::vector<Packet> systemPackets(systemMessages);
    systemMessages.clear();
    return systemPackets;

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
