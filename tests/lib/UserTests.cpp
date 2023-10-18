#include <gtest/gtest.h>
#include "gmock/gmock.h"

#include "User.hpp"  // Your module header

using namespace testing;

TEST(UserTests, getName()) {
    std::string name = "Bob";
    uint32_t id = 1;
    User user(name, id);

    ASSERT_EQ(name, user.getName());
    // ASSERT_EQ(1, function1());  // Replace with actual tests
}

TEST(UserTests, getId()) {
    std::string name = "Bob";
    uint32_t id = 1;
    User user(name, id);

    ASSERT_EQ(id, user.getId());
    // ASSERT_EQ(1, function1());  // Replace with actual tests
}

TEST(UserTests, getMap()) {
    std::string name = "Bob";
    uint32_t id = 1;
    User user(name, id);

    ASSERT_EQ(0, user.getMap());
    // ASSERT_EQ(1, function1());  // Replace with actual tests
}

TEST(UserTests, getRole()) {
    std::string name = "Bob";
    uint32_t id = 1;
    User user(name, id);

    user.setRole(UserRole::Owner);
    ASSERT_EQ(1, user.getRole());
    // ASSERT_EQ(1, function1());  // Replace with actual tests
}