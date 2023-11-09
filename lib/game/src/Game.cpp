#include "Game.hpp"
#include <algorithm>

Game::Game(const Configuration& config) 
    : config(config), isGameRunning(false) {
        this->config = config;
    }

Game::~Game() {}

bool Game::start() {
    if (!isGameRunning && !players.empty()) {
        isGameRunning = true;
        return true;
    }
    return false;
}

bool Game::end() {
    if (isGameRunning) {
        isGameRunning = false;
        return true;
    }
    return false;
}

bool Game::addPlayer(const User& user) {
    if (players.size() < config.playerRange.second) {
        players.push_back(user);

        if (players.size() >= config.playerRange.first) {
            readyToStart = true;
        }

        return true;
    }
    return false;
}

bool Game::removePlayer(const User& user) {
    // Remove players unless the game is running.
    if (isGameRunning) {
        return false;
    }
    
    auto it = std::find(players.begin(), players.end(), user);
    
    if (it == players.end()) {
        return false;
    } else {
        players.erase(it);
        if (players.size() <= config.playerRange.first) {
            readyToStart = false;
        }
    }
    return true;
}

bool Game::addSpectator(const User& user) {
    if (!isAudienceEnabled()) {
        return false;
    }

    spectators.push_back(user);
    return true;
}

bool Game::removeSpectator(const User& user) {
    if (!isAudienceEnabled()) {
        return false;
    }

    auto it = std::find(spectators.begin(), spectators.end(), user);
    if (it == spectators.end()) {
        return false;
    } else {
        spectators.erase(it);
    }
    return true;
}

const std::vector<User>& Game::getPlayers() const {
    return players;
}

const std::vector<User>& Game::getSpectators() const {
    return spectators;
}

bool Game::isAudienceEnabled() const {
    return config.audienceEnabled;
}

bool Game::isRunning() const {
    return isGameRunning;
}

// Why?: To process round based games
void Game::processRound() {
    // Logic for processing a round.
    // ...
}

// Why?: To determine the winner of a round
int Game::determineRoundWinner(const std::vector<std::string>& choices) {
    // Logic to determine the winner based on choices.
    // ...
    return 0;
}

void Game::reportFinalScores() const {
    // Logic to report scores.
    // ...
}
