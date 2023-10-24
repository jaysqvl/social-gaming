#include <cassert>
#include <cstdio>
#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <memory>

#include <cpp-tree-sitter.h>
#include "SyntaxNode.hpp"
#include "SyntaxGenerator.hpp"

// Node Visitor
#include <variant>

namespace Visitor {

struct None;
struct Integer;
struct Boolean;
struct String;
struct Identifier;
struct Range;
struct Pair;
struct List;
struct Dictionary;
using Data = std::variant<
    None, Integer, Boolean,
    String, Identifier, Range,
    Pair, List, Dictionary
>;

struct None {};

struct Integer {
    int value;
};

struct Boolean {
    bool value;
};

struct String {
    std::string value;
};

struct Identifier {
    std::string value;
};

struct Range {
    int begin;
    int end;
};

struct Pair {
    std::shared_ptr<Data> first;
    std::shared_ptr<Data> second;

    Pair(const Data &first, const Data &second) :
        first{std::make_shared<Data>(first)},
        second(std::make_shared<Data>(second)) {}
};

struct List {
    std::vector<Data> value;
};

struct Dictionary {
    std::map<std::string, Data> value;
};

class TreeVisitor {
public:
    TreeVisitor(const std::string &source) :
        source{source} {}
    Data Visit(const ts::Node &node) {
        auto type = node.getType();
        if (type == "game") {
            auto result = Dictionary{};
            size_t numChildren = node.getNumChildren();
            for (size_t i = 0; i < numChildren; i++) {
                auto child = node.getChild(i);
                auto temp = Visit(child);

                if (std::holds_alternative<Pair>(temp)) {
                    auto pair = std::get<Pair>(temp);
                    if (std::holds_alternative<String>(*pair.first)) {
                        auto key = std::get<String>(*pair.first);
                        result.value[key.value] = *pair.second;
                    }
                }
            }
            return result;
        } else if (type == "configuration" ||
                type == "constants" /*||
                type == "variables" ||
                type == "per_player" ||
                type == "per_audience" ||
                type == "rules" */ ||
                type == "setup_rule") {
            if (node.getNumChildren() > 1) {
                auto key = std::string(node.getChild(0).
                        getSourceRange(source));
                ts::Node child = node.getChild(1);
                auto value = VisitSibling(child);
                return Pair(String{key}, value);
            } else {
                return None{};
            }
        } else if (node.getType() == "value_map") {
            auto child = node.getChild(0);
            return VisitSibling(child);
        } else if (type == "map_entry") {
            auto key = std::string(node.getChild(0).
                    getSourceRange(source));
            auto value = Visit(node.getChild(2));
            return Pair{String{key}, value};
        } else if (type == "expression") {
            return Visit(node.getChild(0));
        } else if (type == "list_literal") {
            return Visit(node.getChild(1));
        } else if (type == "expression_list") {
            List result;
            for (size_t i = 0; i < node.getNumChildren(); i += 2) {
                result.value.push_back(Visit(node.getChild(i)));
            }
            return result;
        } else if (node.getType() == "quoted_string") {
            auto temp = std::string(node.getSourceRange(source));
            return String{temp};
        } else if (node.getType() == "number_range") {
            auto begin = std::string(node.getChild(1).
                    getSourceRange(source));
            auto end = std::string(node.getChild(3).
                    getSourceRange(source));
            return Range{std::stoi(begin), std::stoi(end)};
        } else if (node.getType() == "boolean") {
            auto temp = node.getChild(0).getSourceRange(source);
            return Boolean{temp[0] == 't'};
        } else if (node.getType() == "integer") {
            auto temp = std::string(node.getSourceRange(source));
            return Identifier{temp};
        } else if (type == "comment") {
            return None{};
        } else {
            std::cout << "Visit " << node.getType() << std::endl;
            for (size_t i = 0; i < node.getNumChildren(); i++) {
                std::cout << "  " << node.getChild(i).getType() << std::endl;
            }
            return None{};
        }
    }

    Data VisitSibling(ts::Node &node) {
        if (node.getType() == "{") {
            Dictionary result;
            node = node.getNextSibling();
            while (node.getType() != "}") {
                auto temp = VisitSibling(node);

                if (std::holds_alternative<Pair>(temp)) {
                    auto pair = std::get<Pair>(temp);
                    if (std::holds_alternative<String>(*pair.first)) {
                        auto key = std::get<String>(*pair.first);
                        result.value[key.value] = *pair.second;
                    }
                }
            }
            return result;
        } else if (node.getType()[node.getType().size() - 1] == ':') {
            auto key = std::string(node.getType().substr(0, node.getType().size() - 1));
            node = node.getNextSibling();
            auto temp = VisitSibling(node);
            return Pair{String{key}, temp};
        } else if (node.getType() == "setup_rule" ||
                node.getType() == "value_map" ||
                node.getType() == "map_entry" ||
                node.getType() == "quoted_string" ||
                node.getType() == "number_range" ||
                node.getType() == "boolean" ||
                node.getType() == "integer") {
            auto temp = Visit(node);
            node = node.getNextSibling();
            return temp;
        } else {
            std::cout << "Sibling " << node.getType() << std::endl;
            node = node.getNextSibling();
            return None{};
        }
    }
private:
    std::string source;
};

struct Printer {
    void operator()(const None &none) {
        std::cout << "None" << std::endl;
    }
    void operator()(const Integer &integer) {
        std::cout << "Integer " << integer.value << std::endl;
    }
    void operator()(const Boolean &boolean) {
        std::cout << "Boolean " << (boolean.value ? "true" : "false")
            << std::endl;
    }
    void operator()(const String &string) {
        std::cout << "String " << string.value << std::endl;
    }
    void operator()(const Identifier &identifier) {
        std::cout << "Identifier " << identifier.value << std::endl;
    }
    void operator()(const Range &range) {
        std::cout << "Range (" << range.begin << ", "
            << range.end << ")" << std::endl;
    }
    void operator()(const Pair &pair) {
        std::cout << "Pair" << std::endl;
        std::cout << "  ";
        std::visit(*this, *pair.first);
        std::cout << "  ";
        std::visit(*this, *pair.second);
    }
    void operator()(const List &list) {
        std::cout << "List" << std::endl;
        for (const auto &elem : list.value) {
            std::cout << "  ";
            std::visit(*this, elem);
        }
    }
    void operator()(const Dictionary & dict) {
        std::cout << "Dictionary" << std::endl;
        for (const auto &elem : dict.value) {
            std::cout << "  " << elem.first << ": ";
            std::visit(*this, elem.second);
        }
    }
};

};

// Declare an external C function that provides the Tree-sitter language
extern "C" {
TSLanguage* tree_sitter_socialgaming();
}

// Define a function to load the contents of a file into a string
void loadFile(std::string &contents, const std::string &filename) {
    std::ifstream ifs(filename);
    contents.assign(
        std::istreambuf_iterator<char>(ifs),
        std::istreambuf_iterator<char>()
    );
}

int main(int argc, char *argv[]) {
   // Create a language and parser using the Tree-sitter socialgaming language
  ts::Language language = tree_sitter_socialgaming();
  ts::Parser parser{language};

   // Check if a filename argument is provided
  if (argc < 2) {
    // If no filename is provided, return an error code (1)
    return 1;
  }

   // Get the filename from the command line arguments
  std::string filename = std::string(argv[1]);

  // Declare a string to store the source code from the file
  std::string sourcecode;

  // Load the contents of the file into the 'sourcecode' string
  loadFile(sourcecode, filename);

  // Print the source code to the standard output
  // std::cout << sourcecode << std::endl;

  // Parse the source code using the Tree-sitter parser
  ts::Tree tree = parser.parseString(sourcecode);
  ts::Node root = tree.getRootNode();

  // Get the syntax tree in S-expression format and print it
  // auto treestring = root.getSExpr();
  // printf("Syntax tree: %s\n", treestring.get());

  // SyntaxGenerator gen{sourcecode};
  // auto syntax = gen.generate(filename, root);
  // syntax->print(0);

  Visitor::TreeVisitor treeVisitor{sourcecode};
  Visitor::Data result = treeVisitor.Visit(root);

  std::cout << "== Result ==" << std::endl;
  std::visit(Visitor::Printer{}, result);

  // Return 0 to indicate successful execution
  return 0;
}
