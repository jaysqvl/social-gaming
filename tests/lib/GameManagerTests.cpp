#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <iostream>
#include <fstream>
#include <string>

#include <cpp-tree-sitter.h>

#include "GameManager.hpp"

using namespace testing;

TEST(GameManagerTests, AddRmCheckPlayer) {
    User testOwner = User("Bob GM", 0);
    testOwner.setRole(UserRole::Owner);

    GameManager gm("TestGame", testOwner);

    EXPECT_FALSE(gm.hasPlayer("Jerry")); // Checking that Jerry doesn't exist yet

    EXPECT_TRUE(gm.addPlayer("Jerry"));
    EXPECT_TRUE(gm.hasPlayer("Jerry")); // Checking if Jerry was added

    EXPECT_TRUE(gm.removePlayer("Jerry"));
    EXPECT_FALSE(gm.hasPlayer("Jerry")); // Should be EXPECT_FALSE because Jerry should be removed
}


TEST(GameManagerTests, AddRmCheckSpectators) {
    User testOwner = User("Bob GM", 0);
    testOwner.setRole(UserRole::Owner);

    GameManager gm("TestGame", testOwner);
    EXPECT_FALSE(gm.hasAudience());  // Assuming no spectators initially
    
    EXPECT_TRUE(gm.addSpectator("Spectator1"));
    EXPECT_TRUE(gm.hasAudience());  // Checking if Spectator1 was added
    
    EXPECT_TRUE(gm.removeSpectator("Spectator1"));
    EXPECT_FALSE(gm.hasAudience());  // Checking if Spectator1 was removed
}

// More tests can be added though at the moment, GameManager does not have full implementations/setup
// Below are just test stubs that need to be implemented in the future

// NOT TESTABLE YET 
// (Lacks Implementation / Unused Variables / I Lack Understanding of Mocks Potentially?)
/*
TEST(GameManagerTests, GetConstants) {
    // User testOwner = User("Bob GM", "0");
    // testOwner.setRole(UserRole::GAME_OWNER);

    // GameManager gm("TestGame", testOwner);
}

TEST(GameManagerTests, GetVariables) {
    User testOwner = User("Bob GM", "0");
    testOwner.setRole(UserRole::GAME_OWNER);

    GameManager gm("TestGame", testOwner);
}

TEST(GameManagerTests, getSetups) {
    User testOwner = User("Bob GM", "0");
    testOwner.setRole(UserRole::GAME_OWNER);

    GameManager gm("TestGame", testOwner);
}

TEST(GameManagerTests, getRules) {
    User testOwner = User("Bob GM", "0");
    testOwner.setRole(UserRole::GAME_OWNER);

    GameManager gm("TestGame", testOwner);
}

TEST(GameManagerTests, parseAndSetRules()) {
    User testOwner = User("Bob GM", "0");
    testOwner.setRole(UserRole::GAME_OWNER);

    GameManager gm("TestGame", testOwner);
}

*/