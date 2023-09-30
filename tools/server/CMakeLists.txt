add_executable(socialserver
  chatserver.cpp
)

set_target_properties(socialserver
                      PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
                      PREFIX ""
)

find_package(Threads REQUIRED)

target_link_libraries(socialserver
  networking
  manager
  ${CMAKE_THREAD_LIBS_INIT}
)

install(TARGETS socialserver
  RUNTIME DESTINATION bin
)

