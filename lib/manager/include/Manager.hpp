#pragma once

#include "Server.h"

using networking::Server;
using networking::Connection;
using networking::Message;

class Manager {
public:
    virtual void onConnect(Connection c) = 0;
    virtual void onDisconnect(Connection c) = 0;
    virtual void processMessages(Server &server,
            std::deque<std::string> &outgoing,
            const std::deque<Message>& incoming) = 0;
    virtual void buildOutgoing(
            std::deque<Message> &outgoing,
            const std::string &log) = 0;
    virtual bool shouldQuit(void) = 0;
private:
};
