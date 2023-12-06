#pragma once

#include "Server.h"

using networking::Server;
using networking::Connection;
using networking::Message;

// Change - by default, the "connection" field determines the recipient of the Packet, not the sender.

enum class PacketType {
    // NONE,
    // FROM,
    TO
};

struct Packet {
    PacketType type;
    Connection connection;
    std::string text;
};
