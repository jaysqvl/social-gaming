#pragma once

#include "Data.hpp"

#include <cpp-tree-sitter.h>

namespace Visitor {

class TreeVisitor {
public:
    TreeVisitor(const std::string &source);
    Data Visit(const ts::Node &node);
    Data VisitSibling(ts::Node &node);
private:
    std::string source;
};

};
