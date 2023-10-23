#include "SyntaxGenerator.hpp"

#include <iostream>

SyntaxGenerator::SyntaxGenerator(const std::string &source) :
    source{source} {}

std::unique_ptr<SyntaxNode> SyntaxGenerator::generate(
        const std::string & name, const ts::Node &node) {
    auto result = std::make_unique<SyntaxNode>("program", name);

    size_t numChildren = node.getNumChildren();
    for (size_t i = 0; i < numChildren; i++) {
        auto child = node.getChild(i);
        if (child.getType() == "configuration") {
            result->append(generateConfiguration(child));
        } else if (child.getType() != "comment") {
            std::cout << "Root " << child.getType() << std::endl;
        }
    }

    return result;
}

std::unique_ptr<SyntaxNode> SyntaxGenerator::generateConfiguration(
        const ts::Node &node) {
    auto result = std::make_unique<SyntaxNode>("configuration", "");

    size_t numChildren = node.getNumChildren();
    for (size_t i = 0; i < numChildren; i++) {
        auto child = node.getChild(i);
        auto type = child.getType();
        auto value = child.getSourceRange(source);
        if (type[type.size() - 1] == ':') {
            auto next = node.getChild(++i);
            if (next.getType() == "{") {
                result->append(generateField(value, node.getChild(++i)));
            } else {
                result->append(generateField(value, next));
            }
        } else {
            std::cout << "Config " << child.getType() << std::endl;
        }
    }

    return result;
}

std::unique_ptr<SyntaxNode> SyntaxGenerator::generateField(
        std::string_view name, const ts::Node &node) {
    auto result = std::make_unique<SyntaxNode>("field", std::string(name));
    auto type = node.getType();
    if (type == "integer") {
        result->append(std::make_unique<SyntaxNode>("integer",
            std::string(node.getSourceRange(source))));
    } else if (type == "quoted_string") {
        result->append(std::make_unique<SyntaxNode>("string",
            std::string(node.getSourceRange(source))));
    } else if (type == "number_range") {
        auto temp = std::make_unique<SyntaxNode>("range", "");
        temp->append(generateAtom(node.getChild(1)));
        temp->append(generateAtom(node.getChild(3)));
        result->append(std::move(temp));
    } else if (type == "boolean") {
        result->append(std::make_unique<SyntaxNode>("boolean",
            std::string(node.getChild(0).getSourceRange(source))));
    } else if (type == "setup_rule") {
        auto temp = std::make_unique<SyntaxNode>("rule",
            std::string(node.getChild(0).getSourceRange(source)));
        for (size_t i = 2; i < node.getNumChildren() - 1; i++) {
            auto key = node.getChild(i++);
            auto value = node.getChild(i);
            temp->append(generateField(key.getSourceRange(source), value));
        }
        result->append(std::move(temp));
    } else {
        std::cout << "Field " << node.getType() << std::endl;
    }

    return result;
}
std::unique_ptr<SyntaxNode> SyntaxGenerator::generateAtom(
        const ts::Node &node) {
    return std::make_unique<SyntaxNode>(
            std::string(node.getType()),
            std::string(node.getSourceRange(source)));
}
