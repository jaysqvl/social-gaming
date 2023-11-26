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

// This function parses a Message object to find the original sender. This will be used to properly connect people to GameManagers.
Connection getSender(const Message& message){
    return message.connection;
}

// This function creates a new GameManager and puts it into the vector<GameManager> within the GeneralManager object. It also adds the creator to the game.
void GeneralManager::createGame(const std::string_view& gameName, Connection& conn){
    // pass in *this* GeneralManager object to the game
    gm.push_back(std::make_unique<GameManager>(gameName, conn, this));
}

void GeneralManager::joinGame(const std::string_view& gameName, Connection& conn){
    // find the game in the gm list. If it exists, join.
    for (auto game : gm){
        if (game.getGameName() == gameName){
            game.addPlayer(std::to_string(conn.id), conn);
            std::cout << "GeneralManager::Join " << gameName << std::endl;
            return;
        }
    }
    std::cout << "Joining game " << gameName << " failed" << std::endl; 
}

// this function gets all the connections and usernames of people in the same room as an input connection
std::vector<Connection> GeneralManager::getOpponents(Connection& conn){
    for (auto game : gm){
        if (game.hasConnection(conn)){
            return game.getConnections();
        }
    }
}

// This function parses a command from a string_view and stores its elements in a vector.
// It splits the input text by spaces and stores the resulting substrings in the 'elems' vector.
void ParseCommand(std::vector<std::string_view> &elems, const std::string_view &text) {
    size_t start = 1; // Initialize the starting position after the first character (typically '/')
    size_t end = text.find(' ', start); // Find the first space after 'start'

    while (end != std::string_view::npos) {
        // Extract a substring from 'start' to 'end' and add it to 'elems'
        elems.push_back(text.substr(start, end - start));

        start = end + 1; // Update the starting position to the character after the space
        end = text.find(' ', start); // Find the next space
    }

    // If there are remaining characters after the last space, add them to 'elems'
    if (start < text.size()) {
        elems.push_back(text.substr(start));
    }
}

// This function processes incoming messages from clients.
// It checks for commands and processes them or forwards regular messages to other clients.
void GeneralManager::processMessages(Server &server, std::deque<Packet> &outgoing, const std::deque<Message>& incoming) {
    for (const auto& message : incoming) {

        // Check if the message is a command (starts with '/')
        if (message.text[0] == '/') {
            std::vector<std::string_view> elems;
            ParseCommand(elems, FirstLine(message.text)); // Extract command and its arguments
            const auto& command = elems[0]; // The command itself

            // Handle different commands
            if (command == "quit") {

                // remove the client from the appropriate GameManager
                // TODO 

                // Disconnect the client
                server.disconnect(message.connection);

            } else if (command == "shutdown") {
                // Perform a server shutdown
                std::cout << "GeneralManager::Shutdown" << std::endl;
                quit = true;

            } else if (command == "create" && elems.size() >= 2){
                // Creates a new game, and passes in the user who created it. The game name is chosen by the user.
                std::cout << "GeneralManager::Create " << elems[1] << std::endl;
                
                auto userConnection = getSender(message);

                // elems[1] is the game name that the message sender chose.
                createGame(std::string(elems[1]), userConnection);

            } else if (command == "join" && elems.size() >= 2) {
                // Handle the 'join' command by updating the room

                auto userConnection = getSender(message);
                joinGame(elems[1], userConnection);

                // info[message.connection.id].room = std::string(elems[1]);

            } else if (command == "changename" && elems.size() >= 2) {
                // Handle the 'changename' command by updating the username
                std::cout << "GeneralManager::ChangeName " << message.connection.id << " => " << elems[1] << std::endl;
                // info[message.connection.id].username = std::string(elems[1]);
            }

        } else if (!FirstLine(message.text).empty()) {
            // Process regular messages and construct outgoing messages
            // const auto &id = message.connection.id;
            // const auto &name = info[id].username;

            // get the sender of the message
            auto userConnection = getSender(message);

            // get all the connections and usernames that the sender should send to
            auto userOpponents = getOpponents(userConnection);



            buildOutgoing(outgoing, Packet{PacketType::FROM, userConnection, message.text}, userOpponents);

            // std::ostringstream out;
            // out << (name.empty() ? std::to_string(id) : name) << "> " << FirstLine(message.text);
            // outgoing.push_back(Packet{PacketType::FROM, message.connection, out.str()});
        }
    }
}

// // This function builds outgoing messages for clients in the same room as the sender.
// void GeneralManager::buildOutgoing(std::deque<Message> &outgoing, const Packet &packet) {
//     // const auto &room = info[packet.connection.id].room; // Get the room of the sender

//     // Use a range-based for loop to iterate over the 'clients' vector
//     for (auto& client : clients) {
//         // Check if the client is in the same room as the sender
//         if (info[client.id].room == room) {
//             // If so, add the message to the 'outgoing' queue for that client
//             outgoing.push_back({client, packet.text});
//         }
//     }
// }

// this function builds outgoing packets for a list of connections
void GeneralManager::buildOutgoing(std::deque<Message> &outgoing, const Packet &packet, std::vector<Connection> conns) {
    // const auto &room = info[packet.connection.id].room; // Get the room of the sender

    // Use a range-based for loop to iterate over the 'clients' vector
    for (auto& conn : conns) {
        // Check if the client is in the same room as the sender
            // If so, add the message to the 'outgoing' queue for that client
            outgoing.push_back({conn, packet.text});
        }
    }
}

// This function returns the value of the 'quit' member variable, indicating whether the server should quit.
bool GeneralManager::shouldQuit(void) {
    return quit;
}
