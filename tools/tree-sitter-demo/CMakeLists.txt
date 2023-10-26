add_executable(tree-sitter-demo)
target_sources(tree-sitter-demo
    PRIVATE
      demo.cpp
)
target_link_libraries(tree-sitter-demo
  tree-sitter-socialgaming
  cpp-tree-sitter
  syntax
  visitor
)

install(TARGETS tree-sitter-demo
  RUNTIME DESTINATION bin)
