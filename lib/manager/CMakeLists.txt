add_library(manager
  src/GeneralManager.cpp
  src/GameManager.cpp
  src/Element.cpp
  src/Rule.cpp
  src/User.cpp
  src/Setup.cpp
)

target_include_directories(manager
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_target_properties(manager
  PROPERTIES
    LINKER_LANGUAGE CXX
    CXX_STANDARD 17
)

target_link_libraries(manager
  networking
  router
  ${CMAKE_THREAD_LIBS_INIT}
  tree-sitter-socialgaming
  cpp-tree-sitter
)
install(TARGETS manager
  ARCHIVE DESTINATION lib
)
