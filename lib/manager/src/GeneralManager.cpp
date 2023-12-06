#include "GeneralManager.hpp"

#include <iostream>
#include <sstream>
#include <unordered_map>

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
    // Print a message to the console indicating that a client is disconnecting
    std::cout << "GeneralManager::Disconnect " << conn.id << std::endl;

    // Use std::remove_if to move all matching connections (equal to conn) to the end of the vector
    clients.erase(std::remove_if(clients.begin(), clients.end(), [&conn](const Connection& c) { return c == conn; }), clients.end());

    // Find the entry in the 'info' map that corresponds to the disconnected client
    auto info_it = info.find(conn.id);

    // If the entry is found in the 'info' map, erase it
    if (info_it != info.end()) {
        info.erase(info_it);
    }
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
    std::istringstream iss(text.substr(1).data());
    std::copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(), std::back_inserter(elems));
}

// This function processes incoming messages from clients.
// It checks for commands and processes them or forwards regular messages to other clients.
void GeneralManager::processMessages(Server &server, std::deque<Packet> &outgoing, const std::deque<Message>& incoming) {
    std::unordered_map<std::string_view, std::function<void(const Message&)>> commandActions = {
        {"quit", [&](const Message& message) { server.disconnect(message.connection); }},
        {"shutdown", [&](const Message&) { std::cout << "GeneralManager::Shutdown" << std::endl; quit = true; }},
        {"join", [&](const Message& message) {
            auto& room = info[message.connection.id].room;
            if (message.text.size() >= 7) { // Ensuring command has enough characters (including '/join ')
                room = std::string(message.text.substr(6)); // Extract room name
                std::cout << "GeneralManager::Join " << message.connection.id << " " << room << std::endl;
            }
        }},
        {"changename", [&](const Message& message) {
            auto& username = info[message.connection.id].username;
            if (message.text.size() >= 13) { // Ensuring command has enough characters (including '/changename ')
                username = std::string(message.text.substr(12)); // Extract username
                std::cout << "GeneralManager::ChangeName " << message.connection.id << " => " << username << std::endl;
            }
        }},
    };

    for (const auto& message : incoming) {
        if (message.text[0] == '/') {
            std::vector<std::string_view> elems;
            ParseCommand(elems, FirstLine(message.text));
            const auto& command = elems[0];

            if (commandActions.find(command) != commandActions.end()) {
                commandActions[command](message);
            }
        } else if (!FirstLine(message.text).empty()) {
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