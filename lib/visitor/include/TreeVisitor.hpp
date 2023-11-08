#pragma once

#include "Data.hpp"

#include <cpp-tree-sitter.h>

namespace Visitor {

class TreeVisitor {
public:
    TreeVisitor(const std::string &source);
    Data Visit(const ts::Node &node);
    Data VisitSibling(ts::Node &node);
    Data ParseRules(const ts::Node &node);
    Data ParseLoopBody(const ts::Node &node);
    Data ParseMatchCondition(const ts::Node &node);
    Data ParseTrueBranch(const ts::Node &node);
    Data ParseDiscardStatement(const ts::Node &node);


private:
    std::string source;
};

};
