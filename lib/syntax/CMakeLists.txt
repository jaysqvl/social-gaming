add_library(syntax
  src/SyntaxNode.cpp
  src/SyntaxGenerator.cpp
)

target_include_directories(syntax
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_target_properties(syntax
  PROPERTIES
    LINKER_LANGUAGE CXX
    CXX_STANDARD 17
)

target_link_libraries(syntax
  ${CMAKE_THREAD_LIBS_INIT}
  tree-sitter-socialgaming
  cpp-tree-sitter
)
install(TARGETS syntax
  ARCHIVE DESTINATION lib
)
