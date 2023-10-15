#include "Setup.hpp"
#include "json.hpp"

#include <iostream>
#include <fstream>

Setup::Setup(const std::string &gameName, const GameKind &gameKind, const std::string &userPrompt, int defaultChoice):
    name{gameName}, kind{gameKind}, prompt{userPrompt}, defaultOption{defaultChoice} {};

nlohmann::json Setup::parseJsonData(std::string jsonString) {
    // TODO
    return nlohmann::json::parse("{}");
};


