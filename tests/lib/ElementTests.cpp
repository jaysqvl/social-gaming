#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Element.hpp"
#include "User.hpp"
#include "Rule.hpp"

using namespace testing;

TEST(ElementTests, SetGetDataInt) {
    Element<int> elem(5);
    EXPECT_EQ(5, elem.getData());
    elem.setData(10);
    EXPECT_EQ(10, elem.getData());
}

TEST(ElementTests, SetGetDataDouble) {
    Element<double> elem(5.5);
    EXPECT_DOUBLE_EQ(5.5, elem.getData());
    elem.setData(10.1);
    EXPECT_DOUBLE_EQ(10.1, elem.getData());
}

TEST(ElementTests, SetGetDataString) {
    Element<std::string> elem("test");
    EXPECT_EQ("test", elem.getData());
    elem.setData("changed");
    EXPECT_EQ("changed", elem.getData());
}

TEST(ElementTests, SetGetDataBool) {
    Element<bool> elem(true);
    EXPECT_EQ(true, elem.getData());
    elem.setData(false);
    EXPECT_EQ(false, elem.getData());
}

// More tests can be added here for each of the data types