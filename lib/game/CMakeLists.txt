add_library(game
  src/User.cpp
  src/Game.cpp
)

target_include_directories(game
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_target_properties(game
  PROPERTIES
    LINKER_LANGUAGE CXX
    CXX_STANDARD 17
)

target_link_libraries(game
  networking
  router
  visitor
  ${CMAKE_THREAD_LIBS_INIT}
  tree-sitter-socialgaming
  cpp-tree-sitter
)
install(TARGETS game
  ARCHIVE DESTINATION lib
)
