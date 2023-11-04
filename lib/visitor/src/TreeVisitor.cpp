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
};

std::map<std::string_view, VisitType> visitMap = {
    { "game", VisitType::GAME },
    { "configuration", VisitType::PAIR },
    { "constants", VisitType::PAIR },
    { "variables", VisitType::PAIR },
    { "per_player", VisitType::PAIR },
    { "per_audience", VisitType::PAIR },
    { "rules", VisitType::PAIR },
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
};

// Create a map that maps VisitType to member function pointers
std::map<VisitType, Visitor::Data (Visitor::TreeVisitor::*)(const ts::Node &)> visitFunctionMap = {
    {VisitType::GAME, &Visitor::TreeVisitor::VisitGame},
    {VisitType::PAIR, &Visitor::TreeVisitor::VisitPair},
    {VisitType::FIRST_CHILD, &Visitor::TreeVisitor::VisitFirstChild},
    {VisitType::FIRST_SIBLING, &Visitor::TreeVisitor::VisitFirstSibling},
    {VisitType::MAP_ENTRY, &Visitor::TreeVisitor::VisitMapEntry},
    {VisitType::LIST_LITERAL, &Visitor::TreeVisitor::VisitListLiteral},
    {VisitType::EXPRESSION_LIST, &Visitor::TreeVisitor::VisitExpressionList},
    {VisitType::QUOTED_STRING, &Visitor::TreeVisitor::VisitQuotedString},
    {VisitType::NUMBER_RANGE, &Visitor::TreeVisitor::VisitNumberRange},
    {VisitType::BOOLEAN, &Visitor::TreeVisitor::VisitBoolean},
    {VisitType::NUMBER, &Visitor::TreeVisitor::VisitNumber},
    {VisitType::INTEGER, &Visitor::TreeVisitor::VisitInteger}
};

Visitor::Data Visitor::TreeVisitor::Visit(const ts::Node &node) {
    auto itType = visitMap.find(node.getType());
    if (itType == visitMap.end()) {
        std::cout << "Visit " << node.getType() << std::endl;
        return Visitor::None{};
    }

    VisitType type = itType->second;
    auto itFunction = visitFunctionMap.find(type);
    if (itFunction != visitFunctionMap.end()) {
        // Call the corresponding member function based on the VisitType
        auto visitFunction = itFunction->second;
        return (this->*visitFunction)(node);
    } else {
        std::cout << "Visit " << static_cast<int>(type) << " " << node.getType() << std::endl;
        return Visitor::None{};
    }
}


Visitor::Data Visitor::TreeVisitor::VisitGame(const ts::Node &node) {
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
}

Visitor::Data Visitor::TreeVisitor::VisitPair(const ts::Node &node) {
    if (node.getNumChildren() > 1) {
        auto key = std::string(node.getChild(0).getSourceRange(source));
        ts::Node child = node.getChild(1);
        auto value = VisitSibling(child);
        return Visitor::Pair(String{key}, value);
    } else {
        return Visitor::None{};
    }
}

Visitor::Data Visitor::TreeVisitor::VisitFirstChild(const ts::Node &node) {
    return Visit(node.getChild(0));
}

Visitor::Data Visitor::TreeVisitor::VisitFirstSibling(const ts::Node &node) {
    auto child = node.getChild(0);
    return VisitSibling(child);
}

Visitor::Data Visitor::TreeVisitor::VisitMapEntry(const ts::Node &node) {
    auto key = std::string(node.getChild(0).getSourceRange(source));
    auto value = Visit(node.getChild(2));
    return Visitor::Pair{String{key}, value};
}

Visitor::Data Visitor::TreeVisitor::VisitListLiteral(const ts::Node &node) {
    if (node.getNumChildren() == 3) {
        return Visit(node.getChild(1));
    } else {
        return Visitor::List{};
    }
}

Visitor::Data Visitor::TreeVisitor::VisitExpressionList(const ts::Node &node) {
    Visitor::List result;
    for (size_t i = 0; i < node.getNumChildren(); i += 2) {
        result.value.push_back(Visit(node.getChild(i)));
    }
    return result;
}

Visitor::Data Visitor::TreeVisitor::VisitQuotedString(const ts::Node &node) {
    auto temp = std::string(node.getSourceRange(source));
    return Visitor::String{temp};
}

Visitor::Data Visitor::TreeVisitor::VisitNumberRange(const ts::Node &node) {
    auto begin = std::string(node.getChild(1).getSourceRange(source));
    auto end = std::string(node.getChild(3).getSourceRange(source));
    return Visitor::Range{std::stoi(begin), std::stoi(end)};
}

Visitor::Data Visitor::TreeVisitor::VisitBoolean(const ts::Node &node) {
    auto temp = node.getChild(0).getSourceRange(source);
    return Visitor::Boolean{temp[0] == 't'};
}

Visitor::Data Visitor::TreeVisitor::VisitNumber(const ts::Node &node) {
    auto temp = std::string(node.getSourceRange(source));
    return Visitor::Integer{std::stoi(temp)};
}

Visitor::Data Visitor::TreeVisitor::VisitInteger(const ts::Node &node) {
    auto temp = std::string(node.getSourceRange(source));
    return Visitor::Identifier{temp};
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
};

Visitor::Data Visitor::TreeVisitor::VisitSibling(ts::Node &node) {
    auto itType = siblingMap.find(node.getType());
    if (itType == siblingMap.end()) {
        std::cout << "Sibling " << node.getType() << std::endl;

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
    } else {
        std::cout << "Sibling " << type << " " <<
            node.getType() << std::endl;

        node = node.getNextSibling();
        return Visitor::None{};
    }
}
