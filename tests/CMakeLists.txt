#[[ Include GoogleTest headers
include_directories(
    ${PROJECT_SOURCE_DIR}/lib/googletest/googletest/include
    ${PROJECT_SOURCE_DIR}/lib/googletest/googlemock/include
)

# Your test files
set(TEST_FILES
    tools/DemoTests.cpp
    tools/ChatServerTests.cpp
    lib/ElementTests.cpp
    lib/GeneralManagerTests.cpp
    lib/GameManagerTests.cpp
    lib/RuleTests.cpp
    lib/SetupTests.cpp
    lib/UserTests.cpp
)

# Define a test executable
add_executable(runAllTests ${TEST_FILES})

# Link against GoogleTest
target_link_libraries(runAllTests 
    PRIVATE
        gtest
        gmock
        gmock_main
        manager
        web-socket-networking
        router
        syntax)

# Link against your modules (if they are in a library or executable)
# target_link_libraries(runUnitTests ChatServer)
]]