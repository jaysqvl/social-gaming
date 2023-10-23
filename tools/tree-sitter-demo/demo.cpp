#include <cassert>
#include <cstdio>
#include <memory>
#include <string>
#include <iostream>
#include <fstream>

#include <cpp-tree-sitter.h>
#include "SyntaxNode.hpp"
#include "SyntaxGenerator.hpp"

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

    SyntaxGenerator gen{sourcecode};
    auto syntax = gen.generate(filename, root);
    syntax->print(0);

    // Return 0 to indicate successful execution
    return 0;
  }
}
