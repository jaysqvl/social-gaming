#include "TreeVisitor.hpp"

#include <map>
#include <iostream>

Visitor::TreeVisitor::TreeVisitor(const std::string &source) :
    source{source} {}

enum class VisitType {
    GAME,
    PAIR,
    FIRST_CHILD,
    FIRST_SIBLING,
    MAP_ENTRY,
    LIST_LITERAL,
    EXPRESSION_LIST,
    QUOTED_STRING,
    NUMBER_RANGE,
    BOOLEAN,
    NUMBER,
    INTEGER,
    RULES,
};

std::map<std::string_view, VisitType> visitMap = {
    { "game", VisitType::GAME },
    { "configuration", VisitType::PAIR },
    { "constants", VisitType::PAIR },
    { "variables", VisitType::PAIR },
    { "per_player", VisitType::PAIR },
    { "per_audience", VisitType::PAIR },
    // { "rules", VisitType::PAIR },
    { "setup_rule", VisitType::PAIR },
    { "expression", VisitType::FIRST_CHILD },
    { "value_map", VisitType::FIRST_SIBLING },
    { "map_entry", VisitType::MAP_ENTRY },
    { "list_literal", VisitType::LIST_LITERAL },
    { "expression_list", VisitType::EXPRESSION_LIST },
    { "quoted_string", VisitType::QUOTED_STRING },
    { "number_range", VisitType::NUMBER_RANGE },
    { "boolean", VisitType::BOOLEAN },
    { "number", VisitType::NUMBER },
    { "integer", VisitType::INTEGER },
    { "rules", VisitType::RULES },
};

Visitor::Data Visitor::TreeVisitor::Visit(const ts::Node &node) {
    auto itType = visitMap.find(node.getType());
    if (itType == visitMap.end()) {
        std::cout << "Visit " << node.getType() << std::endl;
        /*for (size_t i = 0; i < node.getNumChildren(); i++) {
            std::cout << "  " << node.getChild(i).getType() << std::endl;
        }*/

        return Visitor::None{};
    }

    VisitType type = itType->second;
    if (type == VisitType::GAME) {
        auto result = Visitor::Dictionary{};
        size_t numChildren = node.getNumChildren();
        for (size_t i = 0; i < numChildren; i++) {
            auto child = node.getChild(i);
            auto temp = Visit(child);

            if (std::holds_alternative<Pair>(temp)) {
                auto pair = std::get<Pair>(temp);
                if (std::holds_alternative<String>(*pair.first)) {
                    auto key = std::get<String>(*pair.first);
                    result.value[key.value] = *pair.second;
                }
            }
        }
        return result;
    } else if (type == VisitType::PAIR) {
        if (node.getNumChildren() > 1) {
            auto key = std::string(node.getChild(0).
                    getSourceRange(source));
            ts::Node child = node.getChild(1);
            auto value = VisitSibling(child);
            return Visitor::Pair(String{key}, value);
        } else {
            return Visitor::None{};
        }
    } else if (type == VisitType::FIRST_CHILD) {
        return Visit(node.getChild(0));
    } else if (type == VisitType::FIRST_SIBLING) {
        auto child = node.getChild(0);
        return VisitSibling(child);
    } else if (type == VisitType::MAP_ENTRY) {
        auto key = std::string(node.getChild(0).
                getSourceRange(source));
        auto value = Visit(node.getChild(2));
        return Visitor::Pair{String{key}, value};
    } else if (type == VisitType::LIST_LITERAL) {
        if (node.getNumChildren() == 3) {
            return Visit(node.getChild(1));
        } else {
            return Visitor::List{};
        }
    } else if (type == VisitType::EXPRESSION_LIST) {
        Visitor::List result;
        for (size_t i = 0; i < node.getNumChildren(); i += 2) {
            result.value.push_back(Visit(node.getChild(i)));
        }
        return result;
    } else if (type == VisitType::QUOTED_STRING) {
        auto temp = std::string(node.getSourceRange(source));
        return Visitor::String{temp};
    } else if (type == VisitType::NUMBER_RANGE) {
        auto begin = std::string(node.getChild(1).
                getSourceRange(source));
        auto end = std::string(node.getChild(3).
                getSourceRange(source));
        return Visitor::Range{std::stoi(begin), std::stoi(end)};
    } else if (type == VisitType::BOOLEAN) {
        auto temp = node.getChild(0).getSourceRange(source);
        return Visitor::Boolean{temp[0] == 't'};
    } else if (type == VisitType::NUMBER) {
        auto temp = std::string(node.getSourceRange(source));
        return Visitor::Integer{stoi(temp)};
    } else if (type == VisitType::INTEGER) {
        auto temp = std::string(node.getSourceRange(source));
        return Visitor::Identifier{temp};
    } else if(type == VisitType::RULES) {
        auto key = std::string(node.getChild(0).
                    getSourceRange(source));
        return Visitor::Pair(String{key}, ParseRules(node));
    }

    else {
        std::cout << "Visit " << (int)type << " " <<
            node.getType() << std::endl;

        return Visitor::None{};
    }
}

Visitor::Data Visitor::TreeVisitor::ParseRules(const ts::Node &node) {
    Visitor::List rulesData;
    Visitor::Dictionary discardData;

    // Loop through the children of the "rules" section
    size_t numChildren = node.getNumChildren();
    for (size_t i = 0; i < numChildren; i++) {
        ts::Node child = node.getChild(i);

        if (child.getType() == "for") {
            ts::Node loopBodyNode = child.getChild(4);
            Visitor::Data loopBodyData = ParseLoopBody(loopBodyNode);
            rulesData.value.push_back(loopBodyData);
        }
    }

    return rulesData;
}

Visitor::Data Visitor::TreeVisitor::ParseLoopBody(const ts::Node &node) {
    Visitor::List loopBodyData;

    if (node.getType() == "{") {
        // Loop through the children of the loop body
        size_t numChildren = node.getNumChildren();
        size_t i = 0;

        while (i < numChildren) {
            ts::Node childNode = node.getChild(i);

            // Check for specific constructs within the loop body
            if (childNode.getType() == "match") {
                // Parse the "match" statement
                Visitor::Dictionary matchStatementData;

                // Get the match condition (e.g., !players.elements.weapon.contains(weapon.name))
                ts::Node conditionNode = childNode.getChild(1);
                Visitor::Data conditionData = ParseMatchCondition(conditionNode);

                matchStatementData.value["condition"] = conditionData;

                // Get the true branch
                ts::Node trueBranchNode = childNode.getChild(3);
                Visitor::Data trueBranchData = ParseTrueBranch(trueBranchNode);

                matchStatementData.value["true"] = trueBranchData;
                loopBodyData.value.push_back(matchStatementData);

                i += 4; // Skip "match", condition, "{", and true branch
            } else {
                // Handle other constructs or statements within the loop body as needed
                i++;
            }
        }
    }

    return loopBodyData;
}

Visitor::Data Visitor::TreeVisitor::ParseMatchCondition(const ts::Node &node) {
    std::string conditionText = std::string(node.getSourceRange(source));

    // Tokenize the condition text based on operators and parentheses
    std::vector<std::string> tokens;
    std::string currentToken;
    bool insideParentheses = false;

    for (char c : conditionText) {
        if (c == '(') {
            insideParentheses = true;
        }
        if (c == ')') {
            insideParentheses = false;
        }

        if (c == ' ' && !insideParentheses) {
            if (!currentToken.empty()) {
                tokens.push_back(currentToken);
                currentToken.clear();
            }
        } else {
            currentToken += c;
        }
    }

    if (!currentToken.empty()) {
        tokens.push_back(currentToken);
    }

    // Parse the tokens to extract relevant information
    std::string object;
    std::string method; 
    std::string argument;

    if (tokens.size() >= 4) {
        object = tokens[1];
        method = tokens[2];
        argument = tokens[3];
    }

    Visitor::Dictionary conditionData;
    conditionData.value["object"] = Visitor::String{object};
    conditionData.value["method"] = Visitor::String{method};
    conditionData.value["argument"] = Visitor::String{argument};

    return conditionData;
}

// Visitor::Data Visitor::TreeVisitor::ParseDiscardStatement(const ts::Node &node) {
//     // Handle parsing of "discard" statements within the loop body
// }

Visitor::Data Visitor::TreeVisitor::ParseTrueBranch(const ts::Node &node) {
    Visitor::List trueBranchData;

    if (node.getType() == "{") {
        size_t numChildren = node.getNumChildren();
        size_t i = 0;

        while (i < numChildren) {
            ts::Node childNode = node.getChild(i);

            // TODO: Handle statements within the true branch
            i++;
        }
    }

    return trueBranchData;
}


static const std::map<std::string_view, size_t> siblingMap = {
    { "{", 1 },
    { "}", 2 },
    { "name:", 3 },
    { "player range:", 3 },
    { "audience:", 3 },
    { "setup:", 3 },
    { "kind:", 3 },
    { "prompt:", 3 },
    { "range:", 3 },
    { "value_map", 4 },
    { "quoted_string", 4 },
    { "number_range", 4 },
    { "boolean", 4 },
    { "setup_rule", 4 },
    { "map_entry", 4 },
    { "integer", 4 },
    {"rules", 5},
};

Visitor::Data Visitor::TreeVisitor::VisitSibling(ts::Node &node) {
    auto itType = siblingMap.find(node.getType());
    if (itType == siblingMap.end()) {
        std::cout << "Sibling " << node.getType() << std::endl;
        /*for (size_t i = 0; i < node.getNumChildren(); i++) {
            std::cout << "  " << node.getChild(i).getType() << std::endl;
        }*/

        node = node.getNextSibling();
        return Visitor::None{};
    }

    size_t type = itType->second;
    if (type == 1) {
        Visitor::Dictionary result;
        node = node.getNextSibling();

        itType = siblingMap.find(node.getType());
        while (itType == siblingMap.end() || itType->second != 2) {
            auto temp = VisitSibling(node);
            if (std::holds_alternative<Pair>(temp)) {
                auto pair = std::get<Pair>(temp);
                if (std::holds_alternative<String>(*pair.first)) {
                    auto key = std::get<String>(*pair.first);
                    result.value[key.value] = *pair.second;
                }
            }
            itType = siblingMap.find(node.getType());
        }

        node = node.getNextSibling();
        return result;
    } else if (type == 3) {
        auto strType = node.getType();
        auto key = std::string(strType.substr(0, strType.size() - 1));
        node = node.getNextSibling();
        auto temp = VisitSibling(node);
        return Visitor::Pair{String{key}, temp};
    } else if (type == 4) {
        auto temp = Visit(node);
        node = node.getNextSibling();
        return temp;
    } else if(type == 5) {
        auto temp = Visit(node);
        node = node.getNextSibling();
        return temp;
    }
    
    else {
        std::cout << "Sibling " << type << " " <<
            node.getType() << std::endl;

        node = node.getNextSibling();
        return Visitor::None{};
    }
}
