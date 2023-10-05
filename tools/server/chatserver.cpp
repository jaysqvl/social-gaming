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

    if (gm.shouldQuit() || errorWhileUpdating) {
      break;
    }

    const auto incoming = server.receive();
    std::deque<Message> processed;
    std::deque<Message> outgoing;
    gm.processMessages(server, processed, incoming);
    for (auto &message : processed) {
        gm.buildOutgoing(outgoing, message);
    }
    server.send(outgoing);

    sleep(1);
  }

  return 0;
}

