#pragma once

#include "Server.h"
#include "Packet.hpp"

using networking::Server;
using networking::Connection;
using networking::Message;

class Manager {
public:
    virtual void onConnect(Connection c) = 0;
    virtual void onDisconnect(Connection c) = 0;
    virtual void processMessages(Server &server,
            std::deque<Packet> &outgoing,
            const std::deque<Message>& incoming) = 0;
    virtual void buildOutgoing(
            std::deque<Message> &outgoing,
            const Packet &packet) = 0;
    virtual bool shouldQuit(void) = 0;
private:
};
