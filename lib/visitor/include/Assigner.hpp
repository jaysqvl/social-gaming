#pragma once

#include "Data.hpp"

namespace Visitor {

    class Assigner {
    public:
        Assigner() {}
        void operator()(const None& none);
        void operator()(Integer& integer, int newValue);
        void operator()(Double& floating, double newValue);
        void operator()(Boolean& boolean, bool newValue);
        void operator()(String& string, const string& newValue);
        void operator()(Identifier& identifier, const string& newValue);
        void operator()(Range& range, int newBegin, int newEnd);
        void operator()(Pair& pair, const Visitor::Data& newFirst, const Visitor::Data& newSecond);
        void operator()(List& list, const std::vector< Visitor::Data>& newValue);
        void operator()(Dictionary& dict, const std::map<std::string, Visitor::Data>& newValue);
    private:
        
    };

};