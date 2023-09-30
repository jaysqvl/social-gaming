#include <cassert>
#include <cstdio>
#include <memory>
#include <string>
#include <iostream>
#include <fstream>

#include <cpp-tree-sitter.h>

extern "C" {
TSLanguage* tree_sitter_socialgaming();
}

void loadFile(std::string &contents, const std::string &filename) {
    std::ifstream ifs(filename);
    contents.assign(
        std::istreambuf_iterator<char>(ifs),
        std::istreambuf_iterator<char>()
    );
}

int main(int argc, char *argv[]) {
  // Create a language and parser.
  ts::Language language = tree_sitter_socialgaming();
  ts::Parser parser{language};

  if (argc < 2) {
    return 1;
  }

  std::string filename = std::string(argv[1]);
  std::string sourcecode;
  loadFile(sourcecode, filename);

  std::cout << sourcecode << std::endl;

  ts::Tree tree = parser.parseString(sourcecode);
  ts::Node root = tree.getRootNode();

  auto treestring = root.getSExpr();
  printf("Syntax tree: %s\n", treestring.get());

  return 0;
}
