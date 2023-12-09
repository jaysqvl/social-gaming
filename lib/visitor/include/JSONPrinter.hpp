#pragma once

#include "Data.hpp"
#include <fstream>

namespace Visitor {

    class JSONPrinter {
    public:
        JSONPrinter(std::ostream& outputStream) : out(outputStream), indent(0) {}
        void operator()(const None& none);
        void operator()(const Integer& integer);
        void operator()(const Double& floating);
        void operator()(const Boolean& boolean);
        void operator()(const String& string);
        void operator()(const Identifier& identifier);
        void operator()(const Range& range);
        void operator()(const Pair& pair);
        void operator()(const List& list);
        void operator()(const Dictionary& dict);
    private:
        std::ostream& out;
        int indent;
        void printIndent();
    };

};