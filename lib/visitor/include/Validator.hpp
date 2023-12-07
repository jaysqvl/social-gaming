#pragma once

#include "Data.hpp"

// This class is similar to ==
namespace Visitor { 

    class Validator {
    public:
        Validator() : equal(false) {}
        bool isEqual() {
            bool temp = equal;
            equal = false;
            return temp;
        }
        void operator()(const None& none);
        void operator()(const Integer& integer, const Integer& target);
        void operator()(const Double& floating, const Double& target);
        void operator()(const Boolean& boolean, const Boolean& target);
        void operator()(const String& string, const String& target);
        void operator()(const Identifier& identifier, const Identifier& target);
        void operator()(const Range& range, const Range& target);
        void operator()(const Pair& pair, const Pair& target);
        void operator()(const List& list, const List& target);
        void operator()(const Dictionary& dict, const Dictionary& target);
    private:
        bool equal;
    };

};