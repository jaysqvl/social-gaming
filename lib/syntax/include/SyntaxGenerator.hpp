#pragma once

#include "SyntaxNode.hpp"
#include "cpp-tree-sitter.h"

class SyntaxGenerator {
public:
    SyntaxGenerator(void);
    std::unique_ptr<SyntaxNode> generate(
            const std::string & name, const ts::Node &node);
};
