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

    //functions
    //TODO: make the design so that the below functions can remain private
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

    Data getDictionarySibling(ts::Node &node);
    Data getKVPSibling(ts::Node &node);
    Data getPrimitiveType(ts::Node &node);

    //TODO: implement function to handle case 2 (squiggly closing bracket);
    Data handleClosingBracket(ts::Node &node);

private: //fields
    std::string source;
   
};

};