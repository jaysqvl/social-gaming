add_library(router
  src/Packet.cpp
)

target_include_directories(router
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_target_properties(router
  PROPERTIES
    LINKER_LANGUAGE CXX
    CXX_STANDARD 17
)

target_link_libraries(router
  networking
  ${CMAKE_THREAD_LIBS_INIT}
)
install(TARGETS router
  ARCHIVE DESTINATION lib
)
