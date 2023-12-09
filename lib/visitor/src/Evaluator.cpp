#include "Evaluator.hpp"

#include <iostream>
#include <string>

#include "json.hpp"
using json = nlohmann::json;

// TODO - this has a lot of issues and this file has been deleted and re-written many times. This is kind of a mess right now.

/* 

This is the overall view of the text file for specifying a game:

configuration {
  name: ""
  player range: (0, 0)
  audience: false
  setup: {}
}
constants {}
variables {}
per-player {}
per-audience {}
rules {}

*/

// function that seperates a string into a vector of strings, delimited by spaces

std::vector<std::string> splitString(std::string s){
    std::vector<std::string> words;
    std::istringstream iss(s);

    std::string word;
    while (iss >> word) {
        words.push_back(word);
    }
    return words;
}


// Define data structures to represent game settings and state from parsed file
// TODO - eventually this will need to be moved to the proper game/gamemanager files, but for now we will keep it local.
struct GameConfig {
    std::string name;
    std::pair<int, int> playerRange;
    
    int rounds;
    bool audience;
    // setup is 
    

    // constants should be a json file for easy access to variables

};

// have a set of all objects that can be accessed in the game config

// create a json object given a string input
json::json stringToObject(std::string s){
    return json::parse(s);
}


// Loop handlers

void handleForLoop(std::string loopSyntax){
    // given a loop syntax string, seperate it as required
    //   for << fresh variable >> in << expression evaluating to a list >> {
    // << statement list >>
  //}

    std::vector<std::string> strings = splitString(loopSyntax);

    // recursively access the statement list inside

}

void handleWhileLoop(std::string loopSyntax){

//      while << boolean expression >> {
//     << statement list >>
//   }

}


// This is the code that currently recursively prints a tree. This might have to be changed....

Visitor::Evaluator::Evaluator() :
    depth{0} {};

// TODO - I am not confident that this methhod is a good idea.

void Visitor::Evaluator::printDepth() {
    for (size_t i = 0; i < depth; i++) {
        std::cout << " ";
    }
}

void Visitor::Evaluator::printNewline() {
    if (depth == 0) {
        std::cout << std::endl;
    }
}

void Visitor::Evaluator::operator()(const Visitor::None &none) {
    std::cout << "None";
    printNewline();
}

void Visitor::Evaluator::operator()(const Visitor::Integer &integer) {
    std::cout << "Integer {" << integer.value << "}";
    printNewline();
}

void Visitor::Evaluator::operator()(const Visitor::Boolean &boolean) {
    std::cout << "Boolean{" << (boolean.value ? "true" : "false") << "}";
    printNewline();
}

void Visitor::Evaluator::operator()(const Visitor::String &string) {
    std::cout << "String{" << string.value << "}";
    printNewline();
}

void Visitor::Evaluator::operator()(const Visitor::Identifier &identifier) {
    std::cout << "Identifier{" << identifier.value << "}";
    printNewline();
}

void Visitor::Evaluator::operator()(const Visitor::Range &range) {
    std::cout << "Range{" << range.begin << ", " << range.end << "}";
    printNewline();
}

void Visitor::Evaluator::operator()(const Visitor::Pair &pair) {
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

void Visitor::Evaluator::operator()(const Visitor::List &list) {
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

void Visitor::Evaluator::operator()(const Visitor::Dictionary & dict) {
    std::cout << "Dictionary";
    depth += 2;
    for (const auto &elem : dict.value) {
        std::cout << std::endl;
        printDepth();

        if (elem.first == "configuration:"){

        }

        std::cout << elem.first << ": ";
        std::visit(*this, elem.second);
    }
    depth -= 2;
    printNewline();
}
