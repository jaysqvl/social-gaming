#include "json.hpp"
#include <vector>
#include <string>
#include <map>

class Setup {
private:

    enum GameKind {
        BOOOLEAN,
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
    Setup(std::string name, GameKind kind, std::string prompt, int defaultOption);
    nlohmann::json parseJsonData(std::string jsonString);

};