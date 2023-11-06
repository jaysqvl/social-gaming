#pragma once

namespace Visitor {

    class Extender {
    public:
        Extender() {}
        void operator()(const None& none);
        void operator()(const Integer& integer, int addition); // Act like the operator +
        void operator()(const Double& floating, double addition); // Act like the operator +
        void operator()(const Boolean& boolean, bool addition); // Act like the operator &&
        void operator()(const String& string, const string& addition); 
        void operator()(const String& string, char addition);
        void operator()(const Identifier& identifier, const string& addition);
        void operator()(const Range& range, int additionBegin, int additionEnd); // Widen the range
        void operator()(const Pair& pair);
        void operator()(const List& list, const Visitor::Data& newValue);
        void operator()(const Dictionary& dict, std::string additionKey, Visitor::Data& additionValue);
    private:
        
    };

};