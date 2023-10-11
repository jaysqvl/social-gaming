#include "Element.hpp"
#include <stdint.h>
#include <vector>
#include <string>
#include <variant>
#include <map>
#include <cstdint>

class User {
private:
    std::string name;
    uint32_t id;
    std::map<std::string, ElementPtr> userMap;
    ElementPtr element;
    bool isOwner;

public:
    User(std::string name, uint32_t id);
    std::string getName();
    std::map<std::string, ElementPtr> getMap();
    bool getIsOwner();
};