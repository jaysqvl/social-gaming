#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <limits>

#include "User.hpp"

using namespace testing;

TEST(UserTests, getNameTest) {
    std::string name = "Bob";
    uint32_t id = 1;
    User user(name, id);

    ASSERT_EQ(name, user.getName());
}

TEST(UserTests, getIdTest) {
    std::string name1 = "Bob";
    uint32_t id1 = 1;
    User user1(name1, id1);
    ASSERT_EQ(id1, user1.getId());

    std::string name2 = "David";
    uint32_t id2 = std::numeric_limits<uint32_t>::max();
    User user2(name2, id2);
    ASSERT_EQ(id2, user2.getId());

    std::string name3 = "Barbara";
    uint32_t id3 = std::numeric_limits<uint32_t>::min();
    User user3(name3, id3);
    ASSERT_EQ(id3, user3.getId());
}

TEST(UserTests, getMapTest) {
    std::string name = "Bob";
    uint32_t id = 1;
    User user(name, id);

    ASSERT_EQ(NULL, user.getMap());
}

TEST(UserTests, getRoleTest) {
    std::string name = "Bob";
    uint32_t id = 1;
    User user(name, id);

    ASSERT_EQ(NULL, user.getRole());

    user.setRole(UserRole::Owner);
    ASSERT_EQ(UserRole::Owner, user.getRole());

    user.setRole(UserRole::Player);
    ASSERT_EQ(UserRole::Player, user.getRole());

    user.setRole(UserRole::Audience);
    ASSERT_EQ(UserRole::Audience, user.getRole());
}