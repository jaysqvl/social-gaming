#pragma once

#include "Server.h"
#include "Manager.hpp"
#include "GameManager.hpp"
#include "User.hpp"

#include <vector>
#include <map>

// Resolve circular dependency
class GameManager;

class GeneralManager : public Manager {
public:
    struct ClientInfo {
        std::string room;
        std::string username;
    };


    GeneralManager(void);

    void onConnect(Connection conn) override;
    void onDisconnect(Connection conn) override;
    void processMessages(Server &server, std::deque<Packet> &outgoing, const std::deque<Message>& incoming) override;
    void buildOutgoing(std::deque<Message> &outgoing, const Packet &packet) override;
    bool shouldQuit(void) override;

    // creates game, and passes in client of game owner.
    void createGame(const std::string_view& gameName, Connection& conn);

private:
    std::vector<Connection> clients;
    std::map<uintptr_t, ClientInfo> info;
    bool quit;

    std::vector<std::unique_ptr<GameManager>> gm;
};
