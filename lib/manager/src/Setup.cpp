#include "Setup.hpp"
#include "json.hpp"

#include <iostream>
#include <fstream>

Setup::Setup(std::string gameName, GameKind gameKind, std::string userPrompt, int defaultChoice) {
    name = gameName;
    kind = gameKind;
    prompt = userPrompt;
    defaultOption = defaultChoice;
};

nlohmann::json Setup::parseJsonData(std::string jsonString) {

};


