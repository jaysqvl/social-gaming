#pragma once

#include "Data.hpp"
#include <algorithm>

namespace Visitor {

    class Sorter {
    public:
        Sorter() : asc(true) {}
        Sorter(bool isAsc) : asc(isAsc) {}
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
        bool asc;
    };

};
