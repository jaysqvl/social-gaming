#include "GeneralManager.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <string_view>

// Constructor for the GeneralManager class.
GeneralManager::GeneralManager(void) :
    clients{}, info{}, quit{false} {}


// This function is called when a new client connects to the server.
void GeneralManager::onConnect(Connection conn) {
     // Print a message to the console indicating that a new client is connecting
    std::cout << "GeneralManager::Connect " << conn.id << std::endl;

    // Add the new connection to the 'clients' vector, keeping track of connected clients
    clients.push_back(conn);
}

// This function is called when a client disconnects from the server.
void GeneralManager::onDisconnect(Connection conn) {
    std::cout << "GeneralManager::Disconnect " << conn.id << std::endl;
    clients.erase(std::remove_if(clients.begin(), clients.end(), [&conn](const Connection& c) { return c == conn; }), clients.end());
    info.erase(conn.id);  // Directly erase using conn.id
}

// This function searches for the first line break ('\n') character and returns the substring up to that point.
std::string_view FirstLine(const std::string_view &text) {
    // Find the position of the first newline character, or use the end of the string
    auto lineBreak = text.find('\n');
    
    // Return a string_view from the start of the text up to the first newline character
    return text.substr(0, lineBreak);
}

// This function parses a command from a string_view and stores its elements in a vector.
// It splits the input text by spaces and stores the resulting substrings in the 'elems' vector.
void ParseCommand(std::vector<std::string_view> &elems, const std::string_view &text) {
    std::istringstream iss{std::string(text)};
    std::string token;
    while (iss >> token) {
        elems.push_back(token);
    }
}

struct ParsedCommand {
    std::vector<std::string_view> args;
    Connection connection;
};

// This function processes incoming messages from clients.
// It checks for commands and processes them or forwards regular messages to other clients.
void GeneralManager::processMessages(Server &server, std::deque<Packet> &outgoing, const std::deque<Message>& incoming) {
    // Map of actions associated with command strings
    std::map<std::string_view, std::function<void(const ParsedCommand&)>> actions = {
        {"quit", [this, &server](const ParsedCommand& cmd) {
            server.disconnect(cmd.connection);
        }},
        {"shutdown", [this](const ParsedCommand& cmd) {
            std::cout << "GeneralManager::Shutdown" << std::endl;
            quit = true;
        }},
        // More actions can be added here
    };

    for (const auto& message : incoming) {
        // Check if the message is a command (starts with '/')
        if (message.text[0] == '/') {
            std::vector<std::string_view> elems;
            ParseCommand(elems, FirstLine(message.text)); // Extract command and its arguments
            auto command_it = actions.find(elems[0]);
            if (command_it != actions.end()) {
                // If command found, execute the corresponding action
                command_it->second({elems, message.connection});
            }
        } else {
            // Process regular messages and construct outgoing messages
            const auto &id = message.connection.id;
            const auto &name = info[id].username;
            std::ostringstream out;
            out << (name.empty() ? std::to_string(id) : name) << "> " << FirstLine(message.text);
            outgoing.push_back(Packet{PacketType::FROM, message.connection, out.str()});
        }
    }
}

// This function builds outgoing messages for clients in the same room as the sender.
void GeneralManager::buildOutgoing(std::deque<Message> &outgoing, const Packet &packet) {
    const auto &room = info[packet.connection.id].room; // Get the room of the sender

    // Use a range-based for loop to iterate over the 'clients' vector
    for (auto& client : clients) {
        // Check if the client is in the same room as the sender
        if (info[client.id].room == room) {
            // If so, add the message to the 'outgoing' queue for that client
            outgoing.push_back({client, packet.text});
        }
    }
}

// This function returns the value of the 'quit' member variable, indicating whether the server should quit.
bool GeneralManager::shouldQuit(void) {
    return quit;
}
