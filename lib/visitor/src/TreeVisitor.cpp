#include "TreeVisitor.hpp"

#include <iostream>

Visitor::TreeVisitor::TreeVisitor(const std::string &source) :
    source{source} {}

Visitor::Data Visitor::TreeVisitor::Visit(const ts::Node &node) {
    auto type = node.getType();
    if (type == "game") {
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
    } else if (type == "configuration" ||
            type == "constants" ||
            type == "variables" ||
            type == "per_player" ||
            type == "per_audience" ||
            type == "rules" ||
            type == "setup_rule") {
        if (node.getNumChildren() > 1) {
            auto key = std::string(node.getChild(0).
                    getSourceRange(source));
            ts::Node child = node.getChild(1);
            auto value = VisitSibling(child);
            return Visitor::Pair(String{key}, value);
        } else {
            return Visitor::None{};
        }
    } else if (type == "value_map") {
        auto child = node.getChild(0);
        return VisitSibling(child);
    } else if (type == "map_entry") {
        auto key = std::string(node.getChild(0).
                getSourceRange(source));
        auto value = Visit(node.getChild(2));
        return Visitor::Pair{String{key}, value};
    } else if (type == "expression") {
        return Visit(node.getChild(0));
    } else if (type == "list_literal") {
        if (node.getNumChildren() == 3) {
            return Visit(node.getChild(1));
        } else {
            return Visitor::List{};
        }
    } else if (type == "expression_list") {
        Visitor::List result;
        for (size_t i = 0; i < node.getNumChildren(); i += 2) {
            result.value.push_back(Visit(node.getChild(i)));
        }
        return result;
    } else if (type == "quoted_string") {
        auto temp = std::string(node.getSourceRange(source));
        return Visitor::String{temp};
    } else if (type == "number_range") {
        auto begin = std::string(node.getChild(1).
                getSourceRange(source));
        auto end = std::string(node.getChild(3).
                getSourceRange(source));
        return Visitor::Range{std::stoi(begin), std::stoi(end)};
    } else if (type == "boolean") {
        auto temp = node.getChild(0).getSourceRange(source);
        return Visitor::Boolean{temp[0] == 't'};
    } else if (type == "number") {
        auto temp = std::string(node.getSourceRange(source));
        return Visitor::Integer{stoi(temp)};
    } else if (type == "integer") {
        auto temp = std::string(node.getSourceRange(source));
        return Visitor::Identifier{temp};
    } else if (type == "comment") {
        return Visitor::None{};
    } else {
        std::cout << "Visit " << node.getType() << std::endl;
        for (size_t i = 0; i < node.getNumChildren(); i++) {
            std::cout << "  " << node.getChild(i).getType() << std::endl;
        }
        return Visitor::None{};
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
    } else if (node.getType() == "setup_rule" ||
            node.getType() == "value_map" ||
            node.getType() == "map_entry" ||
            node.getType() == "quoted_string" ||
            node.getType() == "number_range" ||
            node.getType() == "boolean" ||
            node.getType() == "integer") {
        auto temp = Visit(node);
        node = node.getNextSibling();
        return temp;
    } else {
        std::cout << "Sibling " << node.getType() << std::endl;
        node = node.getNextSibling();
        return Visitor::None{};
    }
}

