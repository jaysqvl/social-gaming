#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "GameManager.hpp"
#include "Server.h"

// Mock Server class for testing
class MockServer : public networking::Server {
public:
    MOCK_METHOD(void, disconnect, (const networking::Connection&), (override));
    MOCK_METHOD(void, send, (const std::deque<Message>&), (override));
    // Add other necessary mocks for Server class
};

class GameManagerIOTest : public ::testing::Test {
protected:
    MockServer mockServer;
    GameManager gameManager;

    void SetUp() override {
        // Set up common resources for tests
    }

    void TearDown() override {
        // Clean up common resources
    }
};

TEST_F(GameManagerIOTest, ProcessMessagesValidCommandTest) {
    // Mock incoming messages with a valid command
    std::deque<GameManager::Packet> incomingMessages = {
        {GameManager::PacketType::TO, MockConnection(), "/create Game1"}
    };

    // Expectations for server send method
    EXPECT_CALL(mockServer, send(::testing::_)).Times(1);

    // Call the method to process incoming messages
    gameManager.processMessages(mockServer, std::deque<GameManager::Packet>(), incomingMessages);
}

TEST_F(GameManagerIOTest, ProcessMessagesRegularMessageTest) {
    // Mock incoming messages with a regular message
    std::deque<GameManager::Packet> incomingMessages = {
        {GameManager::PacketType::TO, MockConnection(), "Hello, everyone!"}
    };

    // Expectations for server send method
    EXPECT_CALL(mockServer, send(::testing::_)).Times(1);

    // Call the method to process incoming messages
    gameManager.processMessages(mockServer, std::deque<GameManager::Packet>(), incomingMessages);
}

TEST_F(GameManagerIOTest, ProcessMessagesServerDisconnectTest) {
    // Mock incoming messages to trigger server disconnect
    std::deque<GameManager::Packet> incomingMessages = {
        {GameManager::PacketType::TO, MockConnection(), "/quit"}
    };

    // Expectations for server disconnect method
    EXPECT_CALL(mockServer, disconnect(::testing::_)).Times(1);

    // Call the method to process incoming messages
    gameManager.processMessages(mockServer, std::deque<GameManager::Packet>(), incomingMessages);
}

TEST_F(GameManagerIOTest, ProcessMessagesEmptyMessageTest) {
    // Mock incoming messages with an empty message
    std::deque<GameManager::Packet> incomingMessages = {
        {GameManager::PacketType::TO, MockConnection(), ""}
    };

    // Expectations for server send method (no messages should be sent)
    EXPECT_CALL(mockServer, send(::testing::_)).Times(0);

    // Call the method to process incoming messages
    gameManager.processMessages(mockServer, std::deque<GameManager::Packet>(), incomingMessages);
}

TEST_F(GameManagerIOTest, ProcessMessagesInvalidCommandTest) {
    // Mock incoming messages with an invalid command
    std::deque<GameManager::Packet> incomingMessages = {
        {GameManager::PacketType::TO, MockConnection(), "/invalidCommand"}
    };

    // Expectations for server send method
    EXPECT_CALL(mockServer, send(::testing::_)).Times(1);

    // Call the method to process incoming messages
    gameManager.processMessages(mockServer, std::deque<GameManager::Packet>(), incomingMessages);
}

TEST_F(GameManagerIOTest, ProcessMessagesMultipleMessagesTest) {
    // Mock incoming messages with multiple messages
    std::deque<GameManager::Packet> incomingMessages = {
        {GameManager::PacketType::TO, MockConnection(), "Message 1"},
        {GameManager::PacketType::TO, MockConnection(), "Message 2"}
    };

    // Expectations for server send method
    EXPECT_CALL(mockServer, send(::testing::_)).Times(2);

    // Call the method to process incoming messages
    gameManager.processMessages(mockServer, std::deque<GameManager::Packet>(), incomingMessages);
}

TEST_F(GameManagerIOTest, ProcessMessagesSystemMessageTest) {
    // Mock incoming messages with a system message
    std::deque<GameManager::Packet> incomingMessages = {
        {GameManager::PacketType::TO, MockConnection(), "/systemMessage Welcome to the game!"}
    };

    // Expectations for server send method
    EXPECT_CALL(mockServer, send(::testing::_)).Times(1);

    // Call the method to process incoming messages
    gameManager.processMessages(mockServer, std::deque<GameManager::Packet>(), incomingMessages);
}
