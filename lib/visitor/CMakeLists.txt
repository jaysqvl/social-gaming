add_library(visitor
    src/Data.cpp
    src/Printer.cpp
    src/TreeVisitor.cpp
)

target_include_directories(visitor
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_target_properties(visitor
  PROPERTIES
    LINKER_LANGUAGE CXX
    CXX_STANDARD 17
)

target_link_libraries(visitor
  ${CMAKE_THREAD_LIBS_INIT}
  tree-sitter-socialgaming
  cpp-tree-sitter
)
install(TARGETS visitor
  ARCHIVE DESTINATION lib
)
