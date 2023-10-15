#include "GameManager.hpp"

#include <iostream>
#include <fstream>
#include <string>

#include <cpp-tree-sitter.h>

extern "C" {
    TSLanguage* tree_sitter_socialgaming();
}

// Define a function to load the contents of a file into a string
//incorporate later?
void loadFile(std::string &contents, const std::string &filename) {
    std::ifstream ifs(filename);
    contents.assign(
        std::istreambuf_iterator<char>(ifs),
        std::istreambuf_iterator<char>()
    );
}

void GameManager::parseAndSetRules() {

    // Create a language and parser using the Tree-sitter socialgaming language
    ts::Language language = tree_sitter_socialgaming();
    ts::Parser parser{language};

    std::string rulesTxt = "";
    std::string filename = "test.txt"; // get user input for this eventually

    if (filename.ends_with(".txt")) {
        loadFile(rulesTxt, "test.txt");
    } else {
        //not a .txt file, can't be parsed. do nothing.
        std::cerr << "please enter a valid .txt file." << std::endl;
        return;
    }

    ts::Tree tree = parser.parseString(rulesTxt);
    ts::Node root = tree.getRootNode();

    //TODO: parse the nodes in the created tree into the rules vector
    //in the gamemanager object
    //do we need a submodule for the tree sitter as well?

    auto treestring = root.getSExpr();
    printf("Syntax tree: %s\n", treestring.get());
}