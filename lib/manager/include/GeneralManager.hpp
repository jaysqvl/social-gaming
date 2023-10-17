#pragma once

#include "Server.h"
#include "Manager.hpp"

#include <vector>
#include <map>

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

private:
    std::vector<Connection> clients;
    std::map<uintptr_t, ClientInfo> info;
    bool quit;
};
