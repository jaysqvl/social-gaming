#pragma once

#include "Data.hpp"
#include "Validator.hpp"

namespace Visitor {

    class Searcher {
    public:
        Searcher() : found(false), validator(Visitor::Validator()) {}
        bool isFound() {
            bool temp = found;
            found = false;
            return temp;
        }
        void operator()(const None& none);
        void operator()(const Integer& integer, int target);
        void operator()(const Double& floating, double target);
        void operator()(const Boolean& boolean, bool target);
        void operator()(const String& string, std::string target);
        void operator()(const String& string, char target);
        void operator()(const Identifier& identifier, std::string target);
        void operator()(const Range& range, double target);
        void operator()(const Pair& pair, const Visitor::Data& target);
        void operator()(const List& list, const Visitor::Data& target);
        void operator()(const Dictionary& dict, std::string targetKey);
        void operator()(const Dictionary& dict, const Visitor::Data& targetValue);
    private:
        bool found;
        Visitor::Validator& validator;
    };

};