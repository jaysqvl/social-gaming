#include <vector>
#include <string>
#include <variant>
#include <map>

class Player {
private:
    std::string name;
    std::map<std::string, std::vector<std::variant<int, double, std::string, bool>>> playerMap;
    std::vector<std::variant<int, double, std::string, bool>> playerList;

public:
    Player(std::string name);
    virtual std::string getName() = 0;
    virtual std::map<std::string, std::vector<std::variant<int, double, std::string, bool>>> getMap() = 0;
    virtual std::vector<std::variant<int, double, std::string, bool>> getList() = 0;
};