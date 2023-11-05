#pragma once

#include "Data.hpp"

#include <cpp-tree-sitter.h>

namespace Visitor {

class TreeVisitor {
public:

    //suggestion: 
    //add a new entry point (call it Data Parse(const ts::Node &node) and make Visit and VisitSibling private (for better design))
    TreeVisitor(const std::string &source);
    Data Visit(const ts::Node &node);
    Data VisitSibling(ts::Node &node);

private: //functions
    Data VisitGame(const ts::Node &node);
    Data VisitPair(const ts::Node &node);
    Data VisitFirstChild(const ts::Node &node);
    Data VisitFirstSibling(const ts::Node &node);
    Data VisitMapEntry(const ts::Node &node);
    Data VisitListLiteral(const ts::Node &node);
    Data VisitExpressionList(const ts::Node &node);
    Data VisitQuotedString(const ts::Node &node);
    Data VisitNumberRange(const ts::Node &node);
    Data VisitBoolean(const ts::Node &node);
    Data VisitNumber(const ts::Node &node);
    Data VisitInteger(const ts::Node &node);

private: //fields
    std::string source;
   
};

};