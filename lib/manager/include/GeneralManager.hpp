#pragma once

#include "Server.h"
#include "Manager.hpp"

class GeneralManager : public Manager {
public:
    GeneralManager(void);

    void onConnect(Connection c) override;
    void onDisconnect(Connection c) override;
    void processMessages(Server &server,
            std::deque<std::string> &outgoing,
            const std::deque<Message>& incoming) override;
    void buildOutgoing(
            std::deque<Message> &outgoing,
            const std::string &log) override;
    bool shouldQuit(void) override;
private:
    std::vector<Connection> clients;
    bool quit;
};
