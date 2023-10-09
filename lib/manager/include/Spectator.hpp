#include "Player.hpp"

#include <vector>
#include <string>
#include <variant>
#include <map>

class Spectator : public Player {
private:
    std::string name;
    std::map<std::string, std::vector<std::variant<int, double, std::string, bool>>> spectatorMap;
    std::vector<std::variant<int, double, std::string, bool>> spectatorList;

public:
    Spectator(std::string name);
    std::string getName() override;
    virtual std::map<std::string, std::vector<std::variant<int, double, std::string, bool>>> getMap() override;
    virtual std::vector<std::variant<int, double, std::string, bool>> getList() override;
};