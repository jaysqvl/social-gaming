/////////////////////////////////////////////////////////////////////////////
//                         Single Threaded Networking
//
// This file is distributed under the MIT License. See the LICENSE file
// for details.
/////////////////////////////////////////////////////////////////////////////


#include "Server.h"
#include "GeneralManager.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>


using networking::Server;
using networking::Connection;
using networking::Message;

struct ClientInfo;

const int NUMBER_ROOMS = 500;

// regex to check for string pattern of "join n"
const std::regex pattern("^join (\\d+)$"); // Regular expression to match "join n" where n is an int

// array that contains all clients attached to the server as well as the rooms they belong to
std::vector<ClientInfo> clients;

// array of vector connections for room number. clientRooms[0] are all the clients in room 0.
// THIS IS CURRENTLY NOT USED - for the future
std::vector<Connection> clientRooms[NUMBER_ROOMS];

// map that links connection ids from users to their names for easy identification
std::map<int, std::string> mapUsernames;

// If room == -1, it means client is not attached to a room
struct ClientInfo {
  int room = -1;
  Connection client;
  std::string username;
};
struct ClientInfo;

const int NUMBER_ROOMS = 500;

// regex to check for string pattern of "join n"
const std::regex pattern("^join (\\d+)$"); // Regular expression to match "join n" where n is an int

// array that contains all clients attached to the server as well as the rooms they belong to
std::vector<ClientInfo> clients;

// array of vector connections for room number. clientRooms[0] are all the clients in room 0.
// THIS IS CURRENTLY NOT USED - for the future
std::vector<Connection> clientRooms[NUMBER_ROOMS];

// map that links connection ids from users to their names for easy identification
std::map<int, std::string> mapUsernames;

// If room == -1, it means client is not attached to a room
struct ClientInfo {
  int room = -1;
  Connection client;
  std::string username;
  bool isPlayer = false;
};

void
onConnect(Connection c) {
  std::cout << "New connection found: " << c.id << "\n";
  clients.push_back({-1, c, std::to_string(c.id)});
}

void
onDisconnect(Connection c) {
  std::cout << "Connection lost: " << c.id << "\n";
  // auto eraseBegin = std::remove(std::begin(clients), std::end(clients), c);
  // clients.erase(eraseBegin, std::end(clients));
  
  int it = -1;
  // find c from clients and remove it
  for (int i = 0; i < clients.size(); i++){
    if (clients[i].client == c){
      it = i;
    }
  }
  if (it == -1){
    return;
  }
  clients.erase(clients.begin() + it);

}



// This is a batch of all the messages the server collected in a second. the vector contains
// a pair of messageContent, roomNumber (which room the message should be sent to.)
struct MessageResult {
  std::vector<std::pair<std::string, int>> messageVector;
  bool shouldShutdown;
};

struct MessageSend {
  std::string message;
  int room;
};

// function that manages a given user joining a room
bool joinRoom(Connection c, int roomNumber){
    // update client room in the clients vector
    for (int i = 0; i < clients.size(); i++){
      if (clients[i].client == c){
        clients[i].room = roomNumber;
        return true;
      }
    }
    return false;
}

// finds the room that a client is in
int findRoom(Connection c){
  for (auto client : clients){
    if (c == client.client){
      return client.room;
    }
  }
  return -1;
}

// This is the function that's called when the server recieves any text messages from the client
MessageResult
processMessages(Server& server, const std::deque<Message>& incoming) {
  std::ostringstream result;
  bool quit = false;

  // this is to store "n" from "join n"
  std::smatch matches;

  // vectors for storing messages and room numbers in this batch 
  std::vector<std::pair<std::string, int>> msgRoom;

  for (const auto& message : incoming) {
    if (message.text == "quit") {
      server.disconnect(message.connection);
    } else if (message.text == "shutdown") {
      std::cout << "Shutting down.\n";
      quit = true;
      // if message is in form "join n", extract "n"
    } else if (std::regex_search(message.text, matches, pattern)) {
        int roomInt = std::stoi(matches[1].str());
        if (roomInt < NUMBER_ROOMS){
          // assign client to room
          if (joinRoom(message.connection, roomInt)){
            std::cout << "Client " << message.connection.id << " joined room " << matches[1].str() << " successfully." << "\n";
          } else {
            std::cout << "Client " << message.connection.id << " attempted to join room " << matches[1].str() << "unsuccessfully." << "\n";
          }
        }
    } else if (message.text.find("changename:") == 0) {
      size_t posOfColon = message.text.find(':');
      std::string usernameFromInput = message.text.substr(posOfColon + 2);
      std::cout << "Client name: " << usernameFromInput << std::endl;
      mapUsernames.insert({message.connection.id, usernameFromInput});
      std::string username = mapUsernames.find(message.connection.id)->second; 
      std::cout << username << " was assigned to connection id " << message.connection.id << std::endl;
      //assigns the username to the ClientInfo vector after inserting into map
      for (auto c : clients) {
        if (c.client.id == message.connection.id) {
          c.username = usernameFromInput;
          break;
        } 
      }
    } else if(message.text.find("setIsPlayer:") == 0) {
        std::string isPlayer = message.text.substr(message.text.find(":") + 2);
        std::cout << "Is user player? : " << isPlayer << std::endl;

        for (auto c : clients) {
          if (c.client.id == message.connection.id) {
              c.isPlayer = isPlayer.compare("true") ? true : false;
          } 
        }
    }
    
    else {
      std::string username = mapUsernames.find(message.connection.id)->second; 
      std::cout << "username is " << username << std::endl;
      msgRoom.push_back({mapUsernames.find(message.connection.id)->second + ": " + message.text, findRoom(message.connection)});
    }
  }
  
  // if client belongs to a room, send to those in that room. Else, send it to those not in a room.
  return MessageResult{msgRoom, quit};
}


std::deque<Message>
buildOutgoing(const std::vector<std::pair<std::string, int>>& messageRoom) {
  std::deque<Message> outgoing;

  // TODO - make this more efficient
  // for each message in the batch, if the client is subscribed to that room number, send it to them 
  for (auto msg : messageRoom){
    for (auto client : clients) {
      auto [message, room] = msg;
      
      if (client.room == room){
        outgoing.push_back({client.client, message});
      }
    }
  }
  return outgoing;
}


std::string
getHTTPMessage(const char* htmlLocation) {
  if (access(htmlLocation, R_OK ) != -1) {
    std::ifstream infile{htmlLocation};
    return std::string{std::istreambuf_iterator<char>(infile),
                       std::istreambuf_iterator<char>()};

  }

  std::cerr << "Unable to open HTML index file:\n"
            << htmlLocation << "\n";
  std::exit(-1);
}


int
main(int argc, char* argv[]) {
  if (argc < 3) {
    std::cerr << "Usage:\n  " << argv[0] << " <port> <html response>\n"
              << "  e.g. " << argv[0] << " 4002 ./webchat.html\n";
    return 1;
  }

  const unsigned short port = std::stoi(argv[1]);
  GeneralManager gm;
  Server server{port, getHTTPMessage(argv[2]),
    std::bind(&GeneralManager::onConnect, &gm, std::placeholders::_1),
    std::bind(&GeneralManager::onDisconnect, &gm, std::placeholders::_1)
  };

  for (;;) {
    bool errorWhileUpdating = false;
    try {
      server.update();
    } catch (std::exception& e) {
      std::cerr << "Exception from Server update:\n"
                << " " << e.what() << "\n\n";
      errorWhileUpdating = true;
    }

    const auto incoming = server.receive();
    const auto [msgResult, shouldQuit] = processMessages(server, incoming);
    const auto outgoing = buildOutgoing(msgResult);
    server.send(outgoing);

    if (gm.shouldQuit() || errorWhileUpdating) {
      break;
    }

    sleep(1);
  }

  return 0;
}

