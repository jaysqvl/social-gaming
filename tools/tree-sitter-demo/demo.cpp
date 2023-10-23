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
struct String;
struct Pair;
struct List;
struct Dictionary;
using Data = std::variant<
    None, String,
    Pair, List, Dictionary
>;

struct None {};

struct String {
    std::string value;
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
        } else if (type == "configuration" /*||
                type == "constants" ||
                type == "variables" ||
                type == "per_player" ||
                type == "per_audience" ||
                type == "rules" */) {
            if (node.getNumChildren() > 1) {
                auto key = std::string(node.getChild(0).
                        getSourceRange(source));
                auto value = VisitSibling(node.getChild(1));
                return Pair(String{key}, value);
            } else {
                return None{};
            }
        } else if (type == "comment") {
            return None{};
        } else {
            std::cout << "Visit " << node.getType() << std::endl;
            return None{};
        }
    }
    Data VisitSibling(const ts::Node &node) {
        auto type = node.getType();
        if (type == "{") {
            Dictionary result;
            ts::Node next = node.getNextSibling();
            while (next.getType() != "}") {
                auto key = std::string(next.getSourceRange(source));
                auto child = next.getNextSibling();
                auto temp = VisitSibling(child);
                next = child.getNextSibling();
            }
            return result;
        } else {
            std::cout << "Sibling " << node.getType() << std::endl;
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
    void operator()(const String &string) {
        std::cout << "String " << string.value << std::endl;
    }
    void operator()(const Pair &pair) {
        /*std::cout << "Pair" << std::endl;
        std::cout << "  ";
        std::visit(*this, pair.first);
        std::cout << "  ";
        std::visit(*this, pair.second);*/
    }
    void operator()(const List &list) {
        /*std::cout << "List" << std::endl;
        for (const auto &elem : dict.value) {
            std::cout << "  ";
            std::visit(*this, elem);
        }*/
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
    std::string fileContents;
    
    std::ifstream ifs(filename);

    //only assign file contents if the text file actually exists.
    // if (ifs.is_open()) {
    //   contents.assign(
    //       std::istreambuf_iterator<char>(ifs),
    //       std::istreambuf_iterator<char>()
    //   );
    // }
    while (getline(ifs, fileContents)) {
      //do nothing, let the file read in?
      //std::cout << fileContents;
      contents.append(fileContents);
    }
    
    ifs.close();
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

  if (filename.compare(filename.size() - 4, 4, ".txt") != 0) {
    std::cout << "Only .txt files are able to be parsed." << std::endl;

    //error code where invalid file argument given.
    return -1;
  } else {

    // Declare a string to store the source code from the file
    std::string sourcecode;

    // Load the contents of the file into the 'sourcecode' string
    loadFile(sourcecode, filename);

    // Print the source code to the standard output
    std::cout << sourcecode << std::endl;

    //if the filename input was invalid, the sourcecode will be 
    //empty and an error int will be returned.
    if (sourcecode.empty()) {
      std::cout << "Please provide a valid, existing .txt file to be parsed." << std::endl;
    }

    // Parse the source code using the Tree-sitter parser
    ts::Tree tree = parser.parseString(sourcecode);
    ts::Node root = tree.getRootNode();

    // //Get the syntax tree in S-expression format and print it
    // auto treestring = root.getSExpr();
    // printf("Syntax tree: %s\n", treestring.get());

<<<<<<< HEAD
  // SyntaxGenerator gen{sourcecode};
  // auto syntax = gen.generate(filename, root);
  // syntax->print(0);

  Visitor::TreeVisitor treeVisitor{sourcecode};
  Visitor::Data result = treeVisitor.Visit(root);

  std::cout << "== Result ==" << std::endl;
  std::visit(Visitor::Printer{}, result);
=======
    auto treestring = root.getSExpr();
    printf("Syntax tree: %s\n", treestring.get());
>>>>>>> added code to tree sitter demo to test for understanding
=======
    SyntaxGenerator gen{sourcecode};
    auto syntax = gen.generate(filename, root);
    syntax->print(0);
>>>>>>> cleaned up bug caused when trying to resolve merge conflicts

    // Return 0 to indicate successful execution
    return 0;
  }
}
