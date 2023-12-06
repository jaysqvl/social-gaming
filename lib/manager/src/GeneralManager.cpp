#include "GeneralManager.hpp"

#include <iostream>
#include <sstream>

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

void ParseCommand(std::vector<std::string_view> &elems, const std::string_view &text) {
    std::istringstream iss(text.data() + 1); // Skip the first character '/'
    std::string token;
    while (iss >> token) {
        elems.push_back(token);
    }
}

#include <functional>
#include <unordered_map>

using CommandFunction = std::function<void(Server&, const std::deque<Packet>&, const std::deque<Message>&, const std::vector<std::string_view>&)>;

// Define a map between command strings and their corresponding functions
std::unordered_map<std::string_view, CommandFunction> commandMap = {
    {"quit", [](Server& server, const std::deque<Packet>& outgoing, const std::deque<Message>& incoming, const std::vector<std::string_view>& elems) {
        server.disconnect(incoming.front().connection);
    }},
    {"shutdown", [](Server& server, const std::deque<Packet>& outgoing, const std::deque<Message>& incoming, const std::vector<std::string_view>& elems) {
        std::cout << "GeneralManager::Shutdown" << std::endl;
        // Set the 'quit' flag to true or perform any necessary shutdown actions
        // quit = true;
    }},
    {"join", [](Server& server, const std::deque<Packet>& outgoing, const std::deque<Message>& incoming, const std::vector<std::string_view>& elems) {
        if (elems.size() >= 2) {
            std::cout << "GeneralManager::Join " << incoming.front().connection.id << " " << elems[1] << std::endl;
            // Update the room information
            // info[incoming.front().connection.id].room = std::string(elems[1]);
        }
    }},
    {"changename", [](Server& server, const std::deque<Packet>& outgoing, const std::deque<Message>& incoming, const std::vector<std::string_view>& elems) {
        if (elems.size() >= 2) {
            std::cout << "GeneralManager::ChangeName " << incoming.front().connection.id << " => " << elems[1] << std::endl;
            // Update the username
            // info[incoming.front().connection.id].username = std::string(elems[1]);
        }
    }}
};

void GeneralManager::processMessages(Server &server, std::deque<Packet> &outgoing, const std::deque<Message>& incoming) {
    for (const auto& message : incoming) {
        if (message.text[0] == '/') {
            std::vector<std::string_view> elems;
            ParseCommand(elems, FirstLine(message.text)); // Extract command and its arguments
            const auto& command = elems[0]; // The command itself

            // Look for the command in the map and execute the associated function
            auto it = commandMap.find(command);
            if (it != commandMap.end()) {
                // Pass necessary arguments to the command function
                it->second(server, outgoing, incoming, elems);
            }
        } else if (!FirstLine(message.text).empty()) {
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
