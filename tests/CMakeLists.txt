# Include GoogleTest headers
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# Your test files
set(TEST_FILES
    tools/test_chatserver.cpp
    lib/test_Element.cpp
    lib/test_GeneralManager.cpp
    lib/test_Rule.cpp
)

# Define a test executable
add_executable(runUnitTests ${TEST_FILES})

# Link against GoogleTest
target_link_libraries(runUnitTests gtest gtest_main)

# Link against your modules (if they are in a library or executable)
# target_link_libraries(runUnitTests ChatServer)