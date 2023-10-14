add_library(manager
  src/GeneralManager.cpp
  src/Element.cpp
  src/User.cpp
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
  ${CMAKE_THREAD_LIBS_INIT}
)

install(TARGETS manager
  ARCHIVE DESTINATION lib
)
