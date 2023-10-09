add_library(manager
  src/GeneralManager.cpp
  src/Element.cpp
  src/User.cpp
)

find_package(nlohmann_json REQUIRED)

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
  nlohmann_json::nlohmann_json
)

install(TARGETS manager
  ARCHIVE DESTINATION lib
)
