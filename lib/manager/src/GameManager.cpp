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
GameManager::GameManager(std::string name, User owner) :
    owner(owner),
    gameName(name),
    audience(false),
}

// Add a player with the given name
bool GameManager::addPlayer(std::string name) {
    if (playerMap->size() >= playerRange.first && playerMap->size() < playerRange.second) {
        // Check if the number of players is within the allowed range
        auto result = playerMap->emplace(name, User(name, playerMap->size() + 1));
        return result.second; // Return true if the insertion was successful
    }
    return false; // Return false if the player limit has been reached
}

// Remove a player with the given name
bool GameManager::removePlayer(std::string name) {
    auto it = playerMap->find(name);
    if (it != playerMap->end()) {
        // Check if the player exists
        playerMap->erase(it);
        return true; // Return true if the player was removed
    }
    return false; // Return false if the player doesn't exist
}

// Add a spectator with the given name
bool GameManager::addSpectator(std::string name) {
    if (spectatorMap->find(name) == spectatorMap->end()) {
        spectatorMap->emplace(name, User(name, spectatorMap->size() + 1));
        return true; // Return true if the spectator was added
    }
    return false; // Return false if the spectator already exists
}

// Remove a spectator with the given name
bool GameManager::removeSpectator(std::string name) {
    auto it = spectatorMap->find(name);
    if (it != spectatorMap->end()) {
        spectatorMap->erase(it);
        return true; // Return true if the spectator was removed
    }
    return false; // Return false if the spectator doesn't exist
}

// Check if a player with the given name exists
bool GameManager::hasPlayer(std::string name) {
    return playerMap->find(name) != playerMap->end();
}

// Check if the game has an audience
bool GameManager::hasAudience() {
    return !spectatorMap->empty();
}

std::vector<Config> GameManager::getConfigs() {
    return configs;
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
