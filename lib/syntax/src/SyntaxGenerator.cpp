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
        if (type[type.size() - 1] == ':') {
            auto next = node.getChild(++i);
            if (next.getType() == "{") {
                result->append(generateField(type, node.getChild(++i)));
            } else {
                result->append(generateField(type, next));
            }
        } else {
            // std::cout << child.getType() << std::endl;
        }
    }

    return result;
}

std::unique_ptr<SyntaxNode> SyntaxGenerator::generateField(
        std::string_view name, const ts::Node &node) {
    auto result = std::make_unique<SyntaxNode>("field", std::string(name));

    size_t numChildren = node.getNumChildren();
    for (size_t i = 0; i < numChildren; i++) {
        auto child = node.getChild(i);
        auto type = child.getType();

        if (type == "(") {
            auto temp = std::make_unique<SyntaxNode>("range", "");
            do {
                temp->append(generateAtom(node.getChild(++i)));
            } while (node.getChild(++i).getType() == ",");
            result->append(std::move(temp));
        } else if (type == "false" || type == "true") {
            result->append(generateAtom(child));
        } else {
            std::cout << child.getType() << std::endl;
        }
    }

    return result;
}
std::unique_ptr<SyntaxNode> SyntaxGenerator::generateAtom(
        const ts::Node &node) {
    return std::make_unique<SyntaxNode>(
            std::string(node.getType()),
            std::string(node.getSourceRange(source)),
            0);

SyntaxGenerator::SyntaxGenerator(void) {

}

std::unique_ptr<SyntaxNode> SyntaxGenerator::generate(
        const std::string & name, const ts::Node &node) {
    return std::make_unique<SyntaxNode>("program", name, 0);
            std::string(node.getSourceRange(source));
}
