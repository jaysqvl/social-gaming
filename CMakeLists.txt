cmake_minimum_required(VERSION 3.24)
project(SocialGaming)

CMAKE_POLICY(SET CMP0135 NEW)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")

option(ENABLE_FLUTTER_CLIENT "Build the Flutter client" ON)
option(ENABLE_FTXUI_CLIENT "Build the FTXUI client" ON)
option(ENABLE_NCURSES_CLIENT "Build the NCurses client" ON)

add_subdirectory(lib)
add_subdirectory(tools)
add_subdirectory(static)

add_executable(SocialGaming ${PROJECT_SOURCE_DIR}/bin/SocialGaming/main.cpp)
add_custom_target(copy_static ALL
                    COMMAND ${CMAKE_COMMAND} -E copy_directory 
                    ${CMAKE_CURRENT_LIST_DIR}/static 
                    ${CMAKE_CURRENT_BINARY_DIR}/static
                    COMMENT "Copying static folder"
            )
add_dependencies(SocialGaming copy_static)