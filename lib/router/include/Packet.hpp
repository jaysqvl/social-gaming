#pragma once

#include "Server.h"

using networking::Server;
using networking::Connection;
using networking::Message;

enum class PacketType {
    NONE,
    FROM,
    TO
};

struct Packet {
    PacketType type;
    Connection connection;
    std::string text;
};
