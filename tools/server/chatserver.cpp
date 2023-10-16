///////////////////////////////////////////////////////////////////////////////
//                         Single Threaded Networking
//
// This file is distributed under the MIT License. See the LICENSE file
// for details.
///////////////////////////////////////////////////////////////////////////////

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

// Function to read an HTML file and return its content as a string
std::string getHTTPMessage(const char* htmlLocation) {
  // Check if the HTML file is readable
  if (access(htmlLocation, R_OK ) != -1) {
    // Open the file for reading
    std::ifstream infile{htmlLocation};
     // Read the entire file into a string and return it
    return std::string{std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>()};
  }

  // If the HTML file cannot be opened, print an error message and exit
  std::cerr << "Unable to open HTML index file:\n" << htmlLocation << "\n";
  std::exit(-1);
}

int main(int argc, char* argv[]) {
  // Check if the correct number of command-line arguments is provided
  if (argc < 3) {
    // Print a usage message and exit with a non-zero status
    std::cerr << "Usage:\n  " << argv[0] << " <port> <html response>\n" << "  e.g. " << argv[0] << " 4002 ./webchat.html\n";
    return 1;
  }

  // Parse command-line arguments to get the port and HTML file location
  const unsigned short port = std::stoi(argv[1]);

  // Create a GeneralManager object
  GeneralManager gm;

  // Create a Server object with the specified port and callbacks for connection events
  Server server{
    port,
    getHTTPMessage(argv[2]), // HTML content from the specified file
    std::bind(&GeneralManager::onConnect, &gm, std::placeholders::_1), // Callback for connection
    std::bind(&GeneralManager::onDisconnect, &gm, std::placeholders::_1) // Callback for disconnection
  };

  for (;;) {
    bool errorWhileUpdating = false;

    try {
      // Update the server to handle incoming connections and messages
      server.update();
    } catch (std::exception& e) {
      // Handle exceptions that may occur during the server update
      std::cerr << "Exception from Server update:\n" << " " << e.what() << "\n\n";
      errorWhileUpdating = true;
    }

    // Check if the GeneralManager signals the need to quit or if an error occurred
    if (gm.shouldQuit() || errorWhileUpdating) {
      break;
    }

    // Receive incoming messages from the server
    const auto incoming = server.receive();
    std::deque<Message> processed;
    std::deque<Message> outgoing;

    // Process incoming messages and build outgoing messages
    gm.processMessages(server, processed, incoming);
    for (auto &message : processed) {
        gm.buildOutgoing(outgoing, message);
    }

    // Send outgoing messages to connected clients
    server.send(outgoing);

    // Sleep for 1 second (adjust as needed)
    sleep(1);
  }

  return 0;
}
