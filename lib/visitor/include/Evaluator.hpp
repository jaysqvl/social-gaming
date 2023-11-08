#pragma once

#include "Data.hpp"

namespace Visitor {

class Evaluator {
public:

// a set containing all the constants

    Evaluator();
    
    void printDepth();
    void printNewline();
    void operator()(const None &none);
    void operator()(const Integer &integer);
    void operator()(const Boolean &boolean);
    void operator()(const String &string);
    void operator()(const Identifier &identifier);
    void operator()(const Range &range);
    void operator()(const Pair &pair);
    void operator()(const List &list);
    void operator()(const Dictionary & dict);
private:
    size_t depth;
};

};
