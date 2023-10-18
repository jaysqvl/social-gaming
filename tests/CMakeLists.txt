# Include GoogleTest headers
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

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
add_executable(runUnitTests ${TEST_FILES})

# Link against GoogleTest
target_link_libraries(runUnitTests gtest gtest_main)

# Link against your modules (if they are in a library or executable)
# target_link_libraries(runUnitTests ChatServer)