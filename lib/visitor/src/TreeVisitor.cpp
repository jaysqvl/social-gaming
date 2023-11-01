#include "TreeVisitor.hpp"

#include <iostream>
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
};

std::unordered_map<std::string_view, Visitor::TreeVisitorType> treeVisitorMap = {
    {"game", Visitor::TreeVisitorType::GAME},
    {"configuration", Visitor::TreeVisitorType::CONFIGURATION},
    {"constants", Visitor::TreeVisitorType::CONSTANTS},
    {"variables", Visitor::TreeVisitorType::VARIABLES},
    {"per_player", Visitor::TreeVisitorType::PER_PLAYER},
    {"per_audience", Visitor::TreeVisitorType::PER_AUDIENCE},
    {"rules", Visitor::TreeVisitorType::RULES},
    {"setup_rule", Visitor::TreeVisitorType::SETUP_RULE},
    {"value_map", Visitor::TreeVisitorType::VALUE_MAP},
    {"map_entry", Visitor::TreeVisitorType::MAP_ENTRY},
    {"expression", Visitor::TreeVisitorType::EXPRESSION},
    {"list_literal", Visitor::TreeVisitorType::LIST_LITERAL},
    {"expression_list", Visitor::TreeVisitorType::EXPRESSION_LIST},
    {"quoted_string", Visitor::TreeVisitorType::QUOTED_STRING},
    {"number_range", Visitor::TreeVisitorType::NUMBER_RANGE},
    {"boolean", Visitor::TreeVisitorType::BOOLEAN},
    {"number", Visitor::TreeVisitorType::NUMBER},
    {"integer", Visitor::TreeVisitorType::INTEGER},
    {"comment", Visitor::TreeVisitorType::COMMENT}
};

Visitor::TreeVisitor::TreeVisitor(const std::string &source) :
    source{source} {}

Visitor::Data Visitor::TreeVisitor::Visit(const ts::Node &node) {
    if (treeVisitorMap.find(node.getType()) == treeVisitorMap.end()) {
        std::cerr << "Node type " << node.getType() << "does not exist; please add the node type to the enum and map or fix the type." << std::endl;
        return Visitor::None{};
    } else {
        auto type = node.getType();
        Visitor::TreeVisitorType enumType = treeVisitorMap[node.getType()];
        if (enumType == Visitor::TreeVisitorType::GAME) {
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
        } else if (enumType == Visitor::TreeVisitorType::CONFIGURATION||
                enumType == Visitor::TreeVisitorType::CONSTANTS ||
                enumType == Visitor::TreeVisitorType::VARIABLES ||
                enumType == Visitor::TreeVisitorType::PER_PLAYER ||
                enumType == Visitor::TreeVisitorType::PER_AUDIENCE ||
                enumType == Visitor::TreeVisitorType::RULES ||
                enumType == Visitor::TreeVisitorType::SETUP_RULE) {
            if (node.getNumChildren() > 1) {
                auto key = std::string(node.getChild(0).
                        getSourceRange(source));
                ts::Node child = node.getChild(1);
                auto value = VisitSibling(child);
                return Visitor::Pair(String{key}, value);
            } else {
                return Visitor::None{};
            }
        } else if (enumType == Visitor::TreeVisitorType::VALUE_MAP) {
            auto child = node.getChild(0);
            return VisitSibling(child);
        } else if (enumType == Visitor::TreeVisitorType::MAP_ENTRY) {
            auto key = std::string(node.getChild(0).
                    getSourceRange(source));
            auto value = Visit(node.getChild(2));
            return Visitor::Pair{String{key}, value};
        } else if (enumType == Visitor::TreeVisitorType::EXPRESSION) {
            return Visit(node.getChild(0));
        } else if (enumType == Visitor::TreeVisitorType::LIST_LITERAL) {
            if (node.getNumChildren() == 3) {
                return Visit(node.getChild(1));
            } else {
                return Visitor::List{};
            }
        } else if (enumType == Visitor::TreeVisitorType::EXPRESSION_LIST) {
            Visitor::List result;
            for (size_t i = 0; i < node.getNumChildren(); i += 2) {
                result.value.push_back(Visit(node.getChild(i)));
            }
            return result;
        } else if (enumType == Visitor::TreeVisitorType::QUOTED_STRING) {
            auto temp = std::string(node.getSourceRange(source));
            return Visitor::String{temp};
        } else if (enumType == Visitor::TreeVisitorType::NUMBER_RANGE) {
            auto begin = std::string(node.getChild(1).
                    getSourceRange(source));
            auto end = std::string(node.getChild(3).
                    getSourceRange(source));
            return Visitor::Range{std::stoi(begin), std::stoi(end)};
        } else if (enumType == Visitor::TreeVisitorType::BOOLEAN) {
            auto temp = node.getChild(0).getSourceRange(source);
            return Visitor::Boolean{temp[0] == 't'};
        } else if (enumType == Visitor::TreeVisitorType::NUMBER) {
            auto temp = std::string(node.getSourceRange(source));
            return Visitor::Integer{stoi(temp)};
        } else if (enumType == Visitor::TreeVisitorType::INTEGER) {
            auto temp = std::string(node.getSourceRange(source));
            return Visitor::Identifier{temp};
        } else if (enumType == Visitor::TreeVisitorType::COMMENT) {
            return Visitor::None{};
        } else {
            std::cout << "Visit " << node.getType() << std::endl;
            for (size_t i = 0; i < node.getNumChildren(); i++) {
                std::cout << "  " << node.getChild(i).getType() << std::endl;
            }
            return Visitor::None{};
        }
    }
}

Visitor::Data Visitor::TreeVisitor::VisitSibling(ts::Node &node) {
    if (node.getType() == "{") {
        Visitor::Dictionary result;
        node = node.getNextSibling();
        while (node.getType() != "}") {
            auto temp = VisitSibling(node);

            if (std::holds_alternative<Pair>(temp)) {
                auto pair = std::get<Pair>(temp);
                if (std::holds_alternative<String>(*pair.first)) {
                    auto key = std::get<String>(*pair.first);
                    result.value[key.value] = *pair.second;
                }
            }
        }
        return result;
    } else if (node.getType()[node.getType().size() - 1] == ':') {
        auto key = std::string(node.getType().substr(0,
                node.getType().size() - 1));
        node = node.getNextSibling();
        auto temp = VisitSibling(node);
        return Visitor::Pair{String{key}, temp};
    } else if (treeVisitorMap[node.getType()] == Visitor::TreeVisitorType::SETUP_RULE ||
            treeVisitorMap[node.getType()] == Visitor::TreeVisitorType::VALUE_MAP ||
            treeVisitorMap[node.getType()] == Visitor::TreeVisitorType::MAP_ENTRY ||
            treeVisitorMap[node.getType()] == Visitor::TreeVisitorType::QUOTED_STRING ||
            treeVisitorMap[node.getType()] == Visitor::TreeVisitorType::NUMBER_RANGE ||
            treeVisitorMap[node.getType()] == Visitor::TreeVisitorType::BOOLEAN ||
            treeVisitorMap[node.getType()] == Visitor::TreeVisitorType::INTEGER) {
        auto temp = Visit(node);
        node = node.getNextSibling();
        return temp;
    } else {
        std::cout << "Sibling " << node.getType() << std::endl;
        node = node.getNextSibling();
        return Visitor::None{};
    }
}

