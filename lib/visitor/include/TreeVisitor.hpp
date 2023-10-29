#pragma once

#include "Data.hpp"

#include <cpp-tree-sitter.h>
#include <unordered_map>

namespace Visitor {

//an enum of node types which can be used possibly in the future with the Data class
enum class TreeVisitorType : char {
    GAME,
    CONFIGURATION,
    CONSTANTS,
    VARIABLES,
    PER_PLAYER,
    PER_AUDIENCE,
    RULES,
    SETUP_RULE,
    VALUE_MAP,
    MAP_ENTRY,
    EXPRESSION,
    LIST_LITERAL,
    EXPRESSION_LIST,
    QUOTED_STRING,
    NUMBER_RANGE,
    BOOLEAN,
    NUMBER,
    INTEGER,
    COMMENT,
};

class TreeVisitor {
public:
    TreeVisitor(const std::string &source);
    Data Visit(const ts::Node &node);
    Data VisitSibling(ts::Node &node);
private:
    std::string source;
};

};
