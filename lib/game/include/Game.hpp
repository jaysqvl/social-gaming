#pragma once

#include "User.hpp"
#include "Data.hpp"
#include <vector>

class Game {
private:
    std::vector<User> players;
    std::vector<User> spectators;

    // Use a struct to represent the configuration of the game.
    struct Configuration {
        std::string name;
        std::pair<int, int> playerRange; // (minPlayers, maxPlayers)
        bool audienceEnabled;
        std::vector<std::pair<std::string, std::string>> weapons; // {weaponName, beats}
        int rounds;
        std::vector<int> winners; // Tracks the winners of each round.
    } config;

    bool readyToStart;
    bool isGameRunning;

public:
    // The constructor now takes a configuration struct.
    Game(const Configuration& config);

    // Destructor if needed.
    ~Game();

    // Start and end functions can be simple setters for isGameRunning.
    bool start();
    bool end();

    // Utility functions for managing players and spectators.
    bool addPlayer(const User& user);
    bool removePlayer(const User& user);
    bool addSpectator(const User& user);
    bool removeSpectator(const User& user);

    // Function to process a round of the game.
    void processRound();

    // Function to determine the winner of a round.
    int determineRoundWinner(const std::vector<std::string>& choices);

    // Getter functions for game state.
    const std::vector<User>& getPlayers() const;
    const std::vector<User>& getSpectators() const;
    bool isAudienceEnabled() const;
    bool isRunning() const;

    // Report the final scores.
    void reportFinalScores() const;
};