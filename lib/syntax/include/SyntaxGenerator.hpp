#pragma once

#include "SyntaxNode.hpp"
#include "cpp-tree-sitter.h"

class SyntaxGenerator {
public:
    SyntaxGenerator(const std::string &source);
    std::unique_ptr<SyntaxNode> generate(
            const std::string &name, const ts::Node &node);
private:
    const std::string &source;
private:
    std::unique_ptr<SyntaxNode> generateConfiguration(
            const ts::Node &node);
    std::unique_ptr<SyntaxNode> generateField(
            std::string_view name, const ts::Node &node);
    std::unique_ptr<SyntaxNode> generateAtom(
            const ts::Node &node);
    SyntaxGenerator(void);
    std::unique_ptr<SyntaxNode> generate(
            const std::string & name, const ts::Node &node);
};
