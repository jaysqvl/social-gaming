#include "TreeVisitor.hpp"
#include <map>
#include <iostream>

Visitor::TreeVisitor::TreeVisitor(const std::string &source) :
    source{source} {}

std::map<std::string_view, Visitor::Data (Visitor::TreeVisitor::*)(const ts::Node &)> visitMap = {
    { "game", &Visitor::TreeVisitor::VisitGame},
    { "configuration",  &Visitor::TreeVisitor::VisitPair},
    { "constants", &Visitor::TreeVisitor::VisitPair},
    { "variables",  &Visitor::TreeVisitor::VisitPair},
    { "per_player",  &Visitor::TreeVisitor::VisitPair},
    { "per_audience",  &Visitor::TreeVisitor::VisitPair},
    { "rules", &Visitor::TreeVisitor::VisitPair},
    { "setup_rule",  &Visitor::TreeVisitor::VisitPair},
    { "expression",  &Visitor::TreeVisitor::VisitFirstChild},
    { "value_map",  &Visitor::TreeVisitor::VisitFirstSibling},
    { "map_entry",  &Visitor::TreeVisitor::VisitMapEntry},
    { "list_literal",  &Visitor::TreeVisitor::VisitListLiteral},
    { "expression_list",  &Visitor::TreeVisitor::VisitExpressionList},
    { "quoted_string",  &Visitor::TreeVisitor::VisitQuotedString},
    { "number_range", &Visitor::TreeVisitor::VisitNumberRange},
    { "boolean",  &Visitor::TreeVisitor::VisitBoolean},
    { "number",  &Visitor::TreeVisitor::VisitNumber},
    { "integer", &Visitor::TreeVisitor::VisitInteger},
}; 

//used for things that contain their own data (configuration, strings)
Visitor::Data Visitor::TreeVisitor::Visit(const ts::Node &node) {
    auto itType = visitMap.find(node.getType());
    if (itType == visitMap.end()) {
        std::cout << "Visit " << node.getType() << std::endl;
        return Visitor::None{};
    }

    auto itFunction = visitMap.find(node.getType());
    if (itFunction != visitMap.end()) {
        // Call the corresponding member function based on node type
        auto visitFunction = itFunction->second;
        return (this->*visitFunction)(node);
    } else {
        std::cout << "Visit " << node.getType() << std::endl;
        return Visitor::None{};
    }
}

static const std::map<std::string_view, Visitor::Data (Visitor::TreeVisitor::*)(ts::Node &)> siblingMap = {
    { "{",  &Visitor::TreeVisitor::getDictionarySibling},

    //TODO: needs to be handled in another way
    { "}", &Visitor::TreeVisitor::handleClosingBracket},
    //change the function name and declaration in TreeVisitor.hpp as you see fit

    { "name:", &Visitor::TreeVisitor::getKVPSibling},
    { "player range:", &Visitor::TreeVisitor::getKVPSibling},
    { "audience:", &Visitor::TreeVisitor::getKVPSibling},
    { "setup:", &Visitor::TreeVisitor::getKVPSibling},
    { "kind:", &Visitor::TreeVisitor::getKVPSibling},
    { "prompt:", &Visitor::TreeVisitor::getKVPSibling},
    { "range:", &Visitor::TreeVisitor::getKVPSibling},
    { "value_map", &Visitor::TreeVisitor::getPrimitiveType},
    { "quoted_string", &Visitor::TreeVisitor::getPrimitiveType},
    { "number_range", &Visitor::TreeVisitor::getPrimitiveType},
    { "boolean", &Visitor::TreeVisitor::getPrimitiveType},
    { "setup_rule", &Visitor::TreeVisitor::getPrimitiveType},
    { "map_entry", &Visitor::TreeVisitor::getPrimitiveType},
    { "integer", &Visitor::TreeVisitor::getPrimitiveType},
};

Visitor::Data Visitor::TreeVisitor::VisitSibling(ts::Node &node) {
    auto itFunction = siblingMap.find(node.getType());
    if (itFunction == siblingMap.end()) {
        std::cout << "Sibling " << node.getType() << std::endl;

        node = node.getNextSibling();
        return Visitor::None{};
    } else {
        auto visitFunction = itFunction->second;
        return (this->*visitFunction)(node);
    }
}

Visitor::Data Visitor::TreeVisitor::getDictionarySibling(ts::Node &node) {
    Visitor::Dictionary result;
        node = node.getNextSibling();

        auto itType = siblingMap.find(node.getType());
        //attn Matt - this line used type 2 of the original dict which caused compile errors
        while (itType == siblingMap.end() || itType->second != &Visitor::TreeVisitor::handleClosingBracket) {
        //while (itType == siblingMap.end()) {
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
}

//this means that the key has a "meaningful" sibling value (setup has a meaningful sibling)
//specifically key value pairs
Visitor::Data Visitor::TreeVisitor::getKVPSibling(ts::Node &node) {
    auto strType = node.getType();
    auto key = std::string(strType.substr(0, strType.size() - 1));
    node = node.getNextSibling();
    auto temp = VisitSibling(node);
    return Visitor::Pair{String{key}, temp};
}

//this means that the key itself contains its own meaning (just needs to be visited)
//don't want to visit the sibling, we want it to visit itself.
Visitor::Data Visitor::TreeVisitor::getPrimitiveType(ts::Node &node) {
    auto temp = Visit(node);
    node = node.getNextSibling();
    return temp;
}

//TODO: implement
Visitor::Data Visitor::TreeVisitor::handleClosingBracket(ts::Node &node) {
    return Visitor::None{};
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

//TODO: create a visit for For struct
