#include "JSONPrinter.hpp"

void printIndent() {
    out << "\n";
    for (int i = 0; i < indent; i++) {
        out << "  ";
    }
}

void Visitor::JSONPrinter::operator()(const Visitor::None& none) {
    out << "null";
}

void Visitor::JSONPrinter::operator()(const Visitor::Integer& integer) {
    out << integer.value;
}

void Visitor::JSONPrinter::operator()(const Visitor::String& string) {
    out << "\"" << string.value << "\"";
}

void Visitor::JSONPrinter::operator()(const Visitor::Double& floating) {
    out << floating.value;
}

void Visitor::JSONPrinter::operator()(const Visitor::Boolean& boolean) {
    out << (boolean.value ? "true" : "false");
}

void Visitor::JSONPrinter::operator()(const Visitor::Identifier& identifier) {
    out << "\"" << identifier.value << "\"";
}

void Visitor::JSONPrinter::operator()(const Visitor::Range& range) {
    out << "Range:[" << range.value.begin << ", " << range.value.end << "]";
}

void Visitor::JSONPrinter::operator()(const Visitor::Pair& pair) {
    out << "Pair{" << std::endl;
    indent += 2;
    printIndent();
    out << "first: ";
    print(pair.value.first);
    out << std::endl;
    printIndent();
    out << "second: ";
    print(pair.value.second);
    out << "}" << std::endl;
    indent -= 2;
}

void Visitor::JSONPrinter::operator()(const Visitor::List& list) {
    out << "[";
    indent++;

    for (size_t i = 0; i < list.value.size(); i++) {
        printIndent();
        print(list.value[i]);

        if (i < list.value.size() - 1) {
            out << ",";
        }
    }

    indent--;
    printIndent();
    out << "]";
}

void Visitor::JSONPrinter::operator()(const Visitor::Dictionary& dict) {
    out << "{";
    indent++;

    auto it = dict.value.begin();
    while (it != dict.value.end()) {
        printIndent();
        out << "\"" << it->first << "\": ";
        print(it->second);

        if (++it != object.end()) {
            out << ",";
        }
    }

    indent--;
    printIndent();
    out << "}";
}

