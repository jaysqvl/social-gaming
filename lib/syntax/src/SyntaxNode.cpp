#include "SyntaxNode.hpp"

#include <iostream>

SyntaxNode::SyntaxNode(const std::string &type, const std::string &text) :
    type{type}, text{text} {}


std::string SyntaxNode::getType(void) {
    return type;
}

std::string SyntaxNode::getText(void) {
    return text;
}

size_t SyntaxNode::size(void) {
    return children.size();
}

SyntaxNode &SyntaxNode::getChild(size_t index) {
    return *children[index];
}

void SyntaxNode::append(std::unique_ptr<SyntaxNode> child) {
    children.push_back(std::move(child));
}

void SyntaxNode::print(size_t depth) {
    for (size_t i = 0; i < depth; i++) {
        std::cout << " ";
    }
    std::cout << type << ": " << text << std::endl;

    for (const auto &child : children) {
        child->print(depth + 2);
    }
}
