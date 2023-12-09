#include "Validator.hpp"


void Visitor::Validator::operator()(const None& none) {

}

void Visitor::Validator::operator()(const Integer& integer, const Integer& target) {
    if (integer.value == target.value) {
        equal = true;
    }
}

void Visitor::Validator::operator()(const Double& floating, const Double& target) {
    if (floating.value == target.value) {
        equal = true;
    }
}

void Visitor::Validator::operator()(const Boolean& boolean, const Boolean& target) {
    if (boolean.value == target.value) {
        equal = true;
    }
}

void Visitor::Validator::operator()(const String& string, const String& target) {
    if (str.value == target.value) {
        equal = true;
    }
}

void Visitor::Validator::operator()(const Identifier& identifier, const Identifier& target) {
    if (identifier.value == target.value) {
        equal = true;
    }
}

void Visitor::Validator::operator()(const Range& range, const Range& target) {
    if (range.begin == target.begin && range.end == target.end) {
        equal = true;
    }
}

void Visitor::Validator::operator()(const Pair& pair, const Pair& target) {
    if (pair.first == target.first && pair.second == target.second) {
        equal = true;
    }
}

void Visitor::Validator::operator()(const List& list, const List& target) {
    if (list.value.size() != target.value.size()) {
        return; // Arrays must have the same size
    }

    for (size_t i = 0; i < list.value.size(); i++) {
        std::visit(*this, list.value[i], target.value[i]);
        if (!equal) {
            return; // Element not found or not matching
        }
    }

    equal = true; // All elements matched
}

void Visitor::Validator::operator()(const Dictionary& dict, const Dictionary& target) {
    if (dict.value.size() != target.value.size()) {
        return; // Objects must have the same number of elements
    }

    for (const auto& [key, value] : dict.value) {
        if (target.value.find(key) == target.value.end()) {
            return; // Key not found in target object
        }

        std::visit(*this, value, target.value.at(key));
        if (!equal) {
            return; // Element not found or not matching
        }
    }

    equal = true; // All elements matched
}