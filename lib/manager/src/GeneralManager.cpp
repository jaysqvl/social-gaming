#include "GeneralManager.hpp"

#include <iostream>
#include <sstream>

std::string_view FirstLine(const std::string_view &text) {
    size_t lineBreak = 0;
    while (lineBreak < text.size() && text[lineBreak] != '\n') {
        lineBreak++;
    }
    return text.substr(0, lineBreak);
}

void ParseCommand(std::vector<std::string_view> &elems,
        const std::string_view &text) {
    size_t start = 1;
    for (size_t end = start; end < text.size(); end++) {
        if (end + 1 >= text.size()) {
            elems.push_back(text.substr(start, end - start + 1));
        } else if (text[end] == ' ') {
            elems.push_back(text.substr(start, end - start));
            start = end + 1;
        }
    }
}

GeneralManager::GeneralManager(void) :
    clients{}, info{}, quit{false} {}

void GeneralManager::onConnect(Connection conn) {
    std::cout << "GeneralManager::Connect " << conn.id << std::endl;
    clients.push_back(conn);
}

void GeneralManager::onDisconnect(Connection conn) {
    std::cout << "GeneralManager::Disconnect " << conn.id << std::endl;
    auto eraseBegin = std::remove(
            std::begin(clients), std::end(clients), conn);
    clients.erase(eraseBegin, std::end(clients));

    auto info_it = info.find(conn.id);
    if (info_it != info.end()) {
        info.erase(info_it);
    }
}

void GeneralManager::processMessages(Server &server,
        std::deque<Message> &outgoing,
        const std::deque<Message>& incoming) {
    std::vector<std::string_view> elems;
    for (const auto& message : incoming) {
        elems.clear();
        std::string_view firstLine = FirstLine(message.text);
        if (message.text[0] == '/') {
            ParseCommand(elems, firstLine);
            const auto& command = elems[0];

            if (command == "quit") {
                server.disconnect(message.connection);
            } else if (command == "shutdown") {
                std::cout << "GeneralManager::Shutdown" << std::endl;
                quit = true;
            } else if (command == "join" && elems.size() >= 2) {
                std::cout << "GeneralManager::Join "
                    << message.connection.id << " "
                    << elems[1] << std::endl;
                const auto &id = message.connection.id;
                info[id].room = std::string(elems[1]);
            } else if (command == "changename" && elems.size() >= 2) {
                std::cout << "GeneralManager::ChangeName "
                    << message.connection.id << " => "
                    << elems[1] << std::endl;
                const auto &id = message.connection.id;
                info[id].username = std::string(elems[1]);
            }
        } else if (firstLine.size() > 0) {
            const auto &id = message.connection.id;
            const auto &name = info[id].username;

            std::ostringstream out;
            if (name.size() > 0) {
                out << name;
            } else {
                out << id;
            }
            out << "> " << firstLine;

            outgoing.push_back(Message{message.connection, out.str()});
        }
    }
}

void GeneralManager::buildOutgoing(
        std::deque<Message> &outgoing,
        const Message &message) {
    const auto &room = info[message.connection.id].room;
    for (auto client : clients) {
        if (info[client.id].room == room) {
            outgoing.push_back({client, message.text});
        }
    }
}

bool GeneralManager::shouldQuit(void) {
    return quit;
}
