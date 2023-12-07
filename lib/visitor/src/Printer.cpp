#include "Printer.hpp"

#include <iostream>

Visitor::Printer::Printer() :
    depth{0} {}

void Visitor::Printer::printDepth() {
    for (size_t i = 0; i < depth; i++) {
        std::cout << " ";
    }
}

void Visitor::Printer::printNewline() {
    if (depth == 0) {
        std::cout << std::endl;
    }
}

void Visitor::Printer::operator()(const Visitor::None &none) {
    std::cout << "None";
    printNewline();
}

void Visitor::Printer::operator()(const Visitor::Integer &integer) {
    std::cout << "Integer {" << integer.value << "}";
    printNewline();
}

void Visitor::Printer::operator()(const Visitor::Double& floating) {
    std::cout << "Double {" << floating.value << "}";
    printNewline();
}

void Visitor::Printer::operator()(const Visitor::Boolean &boolean) {
    std::cout << "Boolean{" << (boolean.value ? "true" : "false") << "}";
    printNewline();
}

void Visitor::Printer::operator()(const Visitor::String &string) {
    std::cout << "String{" << string.value << "}";
    printNewline();
}

void Visitor::Printer::operator()(const Visitor::Identifier &identifier) {
    std::cout << "Identifier{" << identifier.value << "}";
    printNewline();
}

void Visitor::Printer::operator()(const Visitor::Range &range) {
    std::cout << "Range{" << range.begin << ", " << range.end << "}";
    printNewline();
}

void Visitor::Printer::operator()(const Visitor::Pair &pair) {
    std::cout << "Pair" << std::endl;
    depth += 2;
    printDepth();
    std::cout << "first: ";
    std::visit(*this, *pair.first);
    std::cout << std::endl;

    printDepth();
    std::cout << "second: ";
    std::visit(*this, *pair.second);
    std::cout << std::endl;
    depth -= 2;
}

void Visitor::Printer::operator()(const Visitor::List &list) {
    std::cout << "List";
    depth += 2;
    for (const auto &elem : list.value) {
        std::cout << std::endl;
        printDepth();
        std::visit(*this, elem);
    }
    printNewline();
    depth -= 2;
}

void Visitor::Printer::operator()(const Visitor::Dictionary & dict) {
    std::cout << "Dictionary";
    depth += 2;
    for (const auto &elem : dict.value) {
        std::cout << std::endl;
        printDepth();
        std::cout << elem.first << ": ";
        std::visit(*this, elem.second);
    }
    depth -= 2;
    printNewline();
}
