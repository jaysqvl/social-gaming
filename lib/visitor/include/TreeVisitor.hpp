#pragma once

#include "Data.hpp"

#include <cpp-tree-sitter.h>

namespace Visitor {

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

// Visitor::TreeVisitorType convertToEnum(std::string input) {
//     switch (input) {
//         case "game" return GAME,
//         case "configuration" return CONFIGURATION,
//         case "constants" return CONSTANTS,
//         case "variables" return VARIABLES,
//         case "per_player" return PER_PLAYER,
//         case "per_audience" return PER_AUDIENCE,
//         case "rules" return RULES,
//         case "setup_rule" return SETUP_RULE,
//         case "value_map" return VALUE_MAP,
//         case "map_entry" return MAP_ENTRY,
//         case "expression" return EXPRESSION,
//         case "list_literal" return LIST_LITERAL,
//         case "expression_list" return EXPRESSION_LIST,
//         case "quoted_string" return QUOTED_STRING,
//         case "number_range" return NUMBER_RANGE,
//         case "boolean" return BOOLEAN,
//         case "number" return NUMBER,
//         case "integer" return INTEGER,
//         case "comment" return COMMENT,    
//     }
// }

class TreeVisitor {
public:
    TreeVisitor(const std::string &source);
    Data Visit(const ts::Node &node);
    Data VisitSibling(ts::Node &node);
private:
    std::string source;
};

};
