cmake_minimum_required(VERSION 3.20)
project(SocialGaming)

include (cmake/CPM.cmake)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")

CPMAddPackage(
  NAME cpp-tree-sitter
  GIT_REPOSITORY https://github.com/nsumner/cpp-tree-sitter.git
  GIT_TAG v0.0.1
)

add_grammar_from_repo(tree-sitter-socialgaming
  https://github.com/nsumner/tree-sitter-socialgaming.git
  0.0.1
)

add_subdirectory(lib)
add_subdirectory(tools)
