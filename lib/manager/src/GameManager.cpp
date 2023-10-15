#include "GameManager.hpp"

// Constructor for the GameManager class
GameManager::GameManager(std::string name, User owner) :
    owner(owner),
    gameName(name),
    hasAudience(false),
    playerRange(std::make_pair(2, 8)) { //Temp playerRange value
    // Initialize the GameManager with the owner and game name
    // Set the initial conditions
    // Adjust playerRange as needed
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