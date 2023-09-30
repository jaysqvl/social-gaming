#include "GeneralManager.hpp"

#include <iostream>
#include <sstream>

GeneralManager::GeneralManager(void) :
    clients{}, quit{false} {}

void GeneralManager::onConnect(Connection c) {
    std::cout << "GeneralManager::Connect " << c.id << std::endl;
    clients.push_back(c);
}

void GeneralManager::onDisconnect(Connection c) {
    std::cout << "GeneralManager::Disconnect " << c.id << std::endl;
    auto eraseBegin = std::remove(
            std::begin(clients), std::end(clients), c);
    clients.erase(eraseBegin, std::end(clients));
}

void GeneralManager::processMessages(Server &server,
        std::deque<std::string> &outgoing,
        const std::deque<Message>& incoming) {
    for (const auto& message : incoming) {
        if (message.text == "quit") {
            server.disconnect(message.connection);
        } else if (message.text == "shutdown") {
            std::cout << "GeneralManager::Shutdown " <<
                message.connection.id << std::endl;
            quit = true;
        } else {
            std::ostringstream out;
            out << message.connection.id << "> " << message.text <<
                std::endl;
            outgoing.push_back(out.str());
        }
    }
}

void GeneralManager::buildOutgoing(
        std::deque<Message> &outgoing,
        const std::string &log) {
    for (auto client : clients) {
        outgoing.push_back({client, log});
    }
}

bool GeneralManager::shouldQuit(void) {
    return quit;
}
