#pragma once

#include "json.hpp"
#include <vector>
#include <string>
#include <map>

class Config {
private:

    enum class GameKind {
        BOOLEAN,
        STRING,
        ENUM,
        QUESTION_ANSWER,
        MULTIPLE_CHOICE,
        JSON
    };

    std::string name;
    GameKind kind;
    std::string prompt;
    std::vector<std::string> choices;
    std::pair<int, int> rangeOption;
    int defaultOption;
    std::map<std::string, std::string> enumOptions;
    nlohmann::json jsonData;

public:
    Config(const std::string &gameName, const GameKind &gameKind, const std::string &userPrompt, int defaultChoice);
    nlohmann::json parseJsonString(const std::string& jsonString);
    nlohmann::json parseJsonFile(const std::string& filename);

};
