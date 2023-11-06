#include "Searcher.hpp"


void Visitor::Searcher::operator()(const None& none) {

}

void Visitor::Searcher::operator()(const Integer& integer, int target) {
    std::visit(validator, integer, target);
    found = validator.isEqual();
}

void Visitor::Searcher::operator()(const Double& floating, double target) {
    std::visit(validator, floating, target);
    found = validator.isEqual();
}

void Visitor::Searcher::operator()(const Boolean& boolean, bool target) {
    std::visit(validator, boolean, target);
    found = validator.isEqual();
}

void Visitor::Searcher::operator()(const String& string, std::string target) {
    if (string.value.size() == target.size()) {
        std::visit(validator, string, target);
        found = validator.isEqual();
    }
    else if (string.value.size() > target.size()) {
        size_t temp = string.value.find(target);
        found = temp != std::string::npos;
    }
    
}

void Visitor::Searcher::operator()(const String& string, char target) {
    size_t temp = string.value.find(target);
    found = temp != std::string::npos;
}

void Visitor::Searcher::operator()(const Identifier& identifier, std::string target) {
    std::visit(validator, identifier, target);
    found = validator.isEqual();
}

void Visitor::Searcher::operator()(const Range& range, double target) {
    if (range.begin <= target && target <= range.end) {
        found = true;
    }
}

void Visitor::Searcher::operator()(const Pair& pair, const Visitor::Data& target) {
    bool temp1, temp2;
    std::visit(validator, pair.begin, target);
    temp1 = validator.isEqual();
    std::visit(validator, pair.end, target);
    temp1 = validator.isEqual();
    found = temp1 || temp2;
}

void Visitor::Searcher::operator()(const List& list, const Visitor::Data& target) {

    for (const auto& element : list.value) {
        std::visit(validator, element, target);
        if (validator.isEqual()) {
            found = true;
            return; 
        }
    }
}

void Visitor::Searcher::operator()(const Dictionary& dict, std::string targetKey) {
    for (const auto& [key, value] : dict.value) {
        std::visit(validator, key, targetKey)
        if (validator.isEqual()) {
            found = true;
            return; 
        }

    }
}

void Visitor::Searcher::operator()(const Dictionary& dict, const Visitor::Data& targetValue) {
    for (const auto& [key, value] : dict.value) {
        std::visit(validator, value, targetValue)
        if (validator.isEqual()) {
            found = true;
            return; 
        }
    }
}