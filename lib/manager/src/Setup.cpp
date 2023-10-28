#include "Setup.hpp"
#include "json.hpp"

#include <iostream>
#include <fstream>

using json = nlohmann::json;

Setup::Setup(const std::string &gameName, const GameKind &gameKind, const std::string &userPrompt, int defaultChoice):
    name{gameName}, kind{gameKind}, prompt{userPrompt}, defaultOption{defaultChoice} {};

json Setup::parseJsonString(const std::string &jsonString) {
    // try {
    //     return json::parse(jsonString);
    // }

    // catch(json::parse_error& e) {
    //     throw std::runtime_error("Error parsing JSON") + std::string(e.what());
    // }
    return nlohmann::json{}; //temp
}


json Setup::parseJsonFile(const std::string &filename) {
    //  std::ifstream file(filename, std::ios_base::binary | std::ios_base::in);

    // if (!file.is_open()) {
    //     throw std::runtime_error("Failed to open " + filename);
    // }

    // // std::string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    // using Iterator = std::istreambuf_iterator<char>;
    // std::string fileContent(Iterator{file}, Iterator{});

    // if (!file) {
    //     throw std::runtime_error("Failed to read " + filename);
    // }

    // file.close();

    // try {
    //     return json::parse(fileContent);
    // }

    // catch(json::parse_error& e) {
    //     throw std::runtime_error("Error parsing JSON") + std::string(e.what());
    // }

    return nlohmann::json{}; //temp

}
