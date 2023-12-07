#include "GeneralManager.hpp"

#include <iostream>
#include <sstream>
#include <unordered_map>


// TODO - THE CURRENT PACKET SENDING METHOD SCALES VERY POORLY. BUT I DON'T CARE ANYMORE.


// Helper functions

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

// End of helper functions

// Constructor for the GeneralManager class.
GeneralManager::GeneralManager(void) :
    clients{}, quit{false} {}


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
    // auto info_it = info.find(conn.id);
    // auto info_it = info.find(conn.id);

    // // If the entry is found in the 'info' map, erase it
    // if (info_it != info.end()) {
    //     info.erase(info_it);
    // }
}



// This function creates a new GameManager and puts it into the vector<GameManager> within the GeneralManager object. It also adds the creator to the game.
void GeneralManager::createGame(const std::string_view& gameName, Connection& conn){
    gm.push_back(std::make_unique<GameManager>(gameName, conn));
}

void GeneralManager::joinGame(const std::string_view& gameName, Connection& conn){
    // find the game in the gm list. If it exists, join.
    for (auto& game : gm){
        if (game->getGameName() == gameName){
            game->addPlayer(conn);
            std::cout << "GeneralManager::Join " << gameName << std::endl;
            return;
        }
    }
    std::cout << "Joining game " << gameName << " failed" << std::endl; 
}

// this function gets all the connections of people in the same room as an input connection
std::vector<Connection> GeneralManager::getOpponents(const Connection& conn){
    for (auto& game : gm){
        if (game->hasConnection(conn)){
            return game->getConnections();
        }
    }
    return {};
}

// this function gets the username of a given connection in a game, if it exists.

std::string GeneralManager::getUsername(const Connection& conn){
    for (auto& game : gm){
        if (game->hasConnection(conn)){
            return game->getUsername(conn);
        }
    }
    return "Null";
    // // If the entry is found in the 'info' map, erase it
    // if (info_it != info.end()) {
    //     info.erase(info_it);
    // }
}



// This function creates a new GameManager and puts it into the vector<GameManager> within the GeneralManager object. It also adds the creator to the game.
void GeneralManager::createGame(const std::string_view& gameName, Connection& conn){
    gm.push_back(std::make_unique<GameManager>(gameName, conn));
}

void GeneralManager::joinGame(const std::string_view& gameName, Connection& conn){
    // find the game in the gm list. If it exists, join.
    for (auto& game : gm){
        if (game->getGameName() == gameName){
            game->addPlayer(conn);
            std::cout << "GeneralManager::Join " << gameName << std::endl;
            return;
        }
    }
    std::cout << "Joining game " << gameName << " failed" << std::endl; 
}

// this function gets all the connections of people in the same room as an input connection
std::vector<Connection> GeneralManager::getOpponents(const Connection& conn){
    for (auto& game : gm){
        if (game->hasConnection(conn)){
            return game->getConnections();
        }
    }
    return {};
}

// this function gets the username of a given connection in a game, if it exists.

std::string GeneralManager::getUsername(const Connection& conn){
    for (auto& game : gm){
        if (game->hasConnection(conn)){
            return game->getUsername(conn);
        }
    }
    return "Null";
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
<<<<<<< HEAD
            ParseCommand(elems, FirstLine(message.text));
            const auto& command = elems[0];
=======
            ParseCommand(elems, FirstLine(message.text)); // Extract command and its arguments
            const auto& command = elems[0]; // The command itself

            // Handle different commands
            if (command == "quit") {

                // TODO - remove the client from the appropriate GameManager


                // TODO - remove the client from the appropriate GameManager

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

            } else if (command == "create" && elems.size() >= 2){
                // Creates a new game, and passes in the user who created it. The game name is chosen by the user.
                std::cout << "GeneralManager::Create " << elems[1] << std::endl;
                
                auto userConnection = getSender(message);

                // elems[1] is the game name that the message sender chose.
                createGame(std::string(elems[1]), userConnection);

            } else if (command == "join" && elems.size() >= 2) {
                // Handle the 'join' command by updating the room

                auto userConnection = getSender(message);
                joinGame(std::string(elems[1]), userConnection);

                // info[message.connection.id].room = std::string(elems[1]);

                auto userConnection = getSender(message);
                joinGame(std::string(elems[1]), userConnection);

                // info[message.connection.id].room = std::string(elems[1]);

            // TODO - this doesn't work.
            // TODO - this doesn't work.
            } else if (command == "changename" && elems.size() >= 2) {
                // Handle the 'changename' command by updating the username
                std::cout << "GeneralManager::ChangeName " << message.connection.id << " => " << elems[1] << std::endl;
                // info[message.connection.id].username = std::string(elems[1]);
                // info[message.connection.id].username = std::string(elems[1]);
            }
>>>>>>> Starting improvements to GameManager, GeneralManager, Game, User in order to get I/O to work

            if (commandActions.find(command) != commandActions.end()) {
                commandActions[command](message);
            }
        } else if (!FirstLine(message.text).empty()) {
            // Process regular messages and construct outgoing messages

            // get the sender of the message
            auto userConnection = getSender(message);

            // get all the connections that the sender should send to
            auto userOpponents = getOpponents(userConnection);

            // get the username of the sender
            auto senderUsername = getUsername(userConnection);

            // create packents for each connection in that room.
            for (const Connection& conn : userOpponents){
                
                std::ostringstream out;
                // out << (name.empty() ? std::to_string(id) : name) << "> " << FirstLine(message.text);
                out << senderUsername << ": > " << FirstLine(message.text);
                outgoing.push_back(Packet{PacketType::TO, conn, out.str()});
            }        

        }
    }

    
    // go through each GameManager and collect the system Packets to send.
    for (auto& game : gm){
        std::vector<Packet> gameSystemMessages = game->retrieveSystemMessages();
        outgoing.insert(outgoing.end(), gameSystemMessages.begin(), gameSystemMessages.end());
    }

    // after processing all incoming user messages, go through each game in the system and retrieve the 
    // system messages it needs to send.

}

// // This function builds outgoing messages for clients in the same room as the sender.

            // get the sender of the message
            auto userConnection = getSender(message);

            // get all the connections that the sender should send to
            auto userOpponents = getOpponents(userConnection);

            // get the username of the sender
            auto senderUsername = getUsername(userConnection);

            // create packents for each connection in that room.
            for (const Connection& conn : userOpponents){
                
                std::ostringstream out;
                // out << (name.empty() ? std::to_string(id) : name) << "> " << FirstLine(message.text);
                out << senderUsername << ": > " << FirstLine(message.text);
                outgoing.push_back(Packet{PacketType::TO, conn, out.str()});
            }        

        }
    }

    
    // go through each GameManager and collect the system Packets to send.
    for (auto& game : gm){
        std::vector<Packet> gameSystemMessages = game->retrieveSystemMessages();
        outgoing.insert(outgoing.end(), gameSystemMessages.begin(), gameSystemMessages.end());
    }

    // after processing all incoming user messages, go through each game in the system and retrieve the 
    // system messages it needs to send.

}

// // This function builds outgoing messages for clients in the same room as the sender.
void GeneralManager::buildOutgoing(std::deque<Message> &outgoing, const Packet &packet) {
    // const auto &room = info[packet.connection.id].room; // Get the room of the sender

    // look at the packet, retrieve the recipient, create a message, and send it to them.
    outgoing.push_back({packet.connection, packet.text});
    // const auto &room = info[packet.connection.id].room; // Get the room of the sender

    // look at the packet, retrieve the recipient, create a message, and send it to them.
    outgoing.push_back({packet.connection, packet.text});

    // Use a range-based for loop to iterate over the 'clients' vector
    // for (auto& client : clients) {
    //     // Check if the client is in the same room as the sender
    //     if (info[client.id].room == room) {
    //         // If so, add the message to the 'outgoing' queue for that client
    //         outgoing.push_back({client, packet.text});
    //     }
    // }
    // for (auto& client : clients) {
    //     // Check if the client is in the same room as the sender
    //     if (info[client.id].room == room) {
    //         // If so, add the message to the 'outgoing' queue for that client
    //         outgoing.push_back({client, packet.text});
    //     }
    // }
}

// void GeneralManager::buildOutgoing(std::deque<Message> &outgoing, const Packet &packet) {
//     // this is called in the chatserver.cpp. Basically, it is passed a packet, and the packet should be changed into a message and added to outgoing.
    
//     // TODO - instead, push to each client in the room.
    
//     outgoing.push_back(Message{packet.connection, packet.text});
// }

// void GeneralManager::buildOutgoing(std::deque<Message> &outgoing, const Packet &packet) {
//     // this is called in the chatserver.cpp. Basically, it is passed a packet, and the packet should be changed into a message and added to outgoing.
    
//     // TODO - instead, push to each client in the room.
    
//     outgoing.push_back(Message{packet.connection, packet.text});
// }

// This function returns the value of the 'quit' member variable, indicating whether the server should quit.
bool GeneralManager::shouldQuit(void) {
    return quit;
}