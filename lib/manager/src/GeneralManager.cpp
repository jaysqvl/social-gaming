#include "GeneralManager.hpp"

#include <iostream>
#include <sstream>
#include <unordered_map>


// TODO - THE CURRENT PACKET SENDING METHOD SCALES QUITE POORLY. BUT I DON'T CARE ANYMORE.

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
    
    for (auto& game : gm){
        game->removeConnection(conn);
    }
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

bool GeneralManager::changeUsername(const std::string& name, const Connection& conn){
    for (auto& game : gm){
        if (game->hasConnection(conn)){
            return game->changePlayerName(conn, name);
        }
    }
    return false;
}

// This function parses a command from a string_view and stores its elements in a vector.
// It splits the input text by spaces and stores the resulting substrings in the 'elems' vector.
// The updated version was not working.

void ParseCommand(std::vector<std::string_view> &elems, const std::string_view &text) {
    size_t start = text.find_first_not_of(' ');
    size_t end = text.find_first_of(' ', start);

    while (start != std::string_view::npos) {
        elems.push_back(text.substr(start, end - start));
        start = text.find_first_not_of(' ', end);
        end = text.find_first_of(' ', start);
    }

    if (!elems.empty()) {
        auto& first_elem = elems.front();
        if (!first_elem.empty()) {
            first_elem.remove_prefix(1);
        }
    }
}


// This function processes incoming messages from clients.
// It checks for commands and processes them or forwards regular messages to other clients.
void GeneralManager::processMessages(Server &server, std::deque<Packet> &outgoing, const std::deque<Message>& incoming) {

    // Sorry, David
    // std::unordered_map<std::string_view, std::function<void(const Message&)>> commandActions = {
    //     {"quit", [&](const Message& message) { server.disconnect(message.connection); }},
    //     {"shutdown", [&](const Message&) { std::cout << "GeneralManager::Shutdown" << std::endl; quit = true; }},
    //     {"join", [&](const Message& message) {
    //         auto& room = info[message.connection.id].room;
    //         if (message.text.size() >= 7) { // Ensuring command has enough characters (including '/join ')
    //             room = std::string(message.text.substr(6)); // Extract room name
    //             std::cout << "GeneralManager::Join " << message.connection.id << " " << room << std::endl;
    //         }
    //     }},
    //     {"changename", [&](const Message& message) {
    //         auto& username = info[message.connection.id].username;
    //         if (message.text.size() >= 13) { // Ensuring command has enough characters (including '/changename ')
    //             username = std::string(message.text.substr(12)); // Extract username
    //             std::cout << "GeneralManager::ChangeName " << message.connection.id << " => " << username << std::endl;
    //         }
    //     }},
    // };

    for (const auto& message : incoming) {

        if (message.text[0] == '/') {
            std::vector<std::string_view> elems;
            ParseCommand(elems, FirstLine(message.text)); // Extract command and its arguments
            const auto& command = elems[0]; // The command itself

            // Handle different commands
            if (command == "quit") {
                // Disconnect the client
                auto userConnection = getSender(message);

                for (auto& game : gm){
                    game->removeConnection(userConnection);
                }

                server.disconnect(userConnection);

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

            // TODO - make sure you can only join one game at a time.
            } else if (command == "join" && elems.size() >= 2) {
                // Handle the 'join' command by updating the room
                auto userConnection = getSender(message);
                joinGame(std::string(elems[1]), userConnection);

            } else if (command == "changename" && elems.size() >= 2) {

                auto userConnection = getSender(message);
                changeUsername(std::string(elems[1]), userConnection);

                std::cout << "GeneralManager::ChangeName " << message.connection.id << " => " << elems[1] << std::endl;
            }

            // if (commandActions.find(command) != commandActions.end()) {
            //     commandActions[command](message);
            // }

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
                out << senderUsername << ": " << FirstLine(message.text);
                outgoing.push_back(Packet{PacketType::TO, conn, out.str()});
            }        

        }
    }

    
    // go through each GameManager and collect the system Packets to send.
    for (auto& game : gm){
        std::vector<Packet> gameSystemMessages = game->retrieveSystemMessages();
        outgoing.insert(outgoing.end(), gameSystemMessages.begin(), gameSystemMessages.end());
    }

}

// // This function builds outgoing messages for clients in the same room as the sender.
void GeneralManager::buildOutgoing(std::deque<Message> &outgoing, const Packet &packet) {

    // look at the packet, retrieve the recipient, create a message, and send it to them.
    outgoing.push_back({packet.connection, packet.text});

}


// This function returns the value of the 'quit' member variable, indicating whether the server should quit.
bool GeneralManager::shouldQuit(void) {
    return quit;
}