#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "server.hpp"
#include "service-exception.hpp"
#include "utils.hpp"

namespace ipc_communication_service {

bool makefifo(const std::string& pathname) {
    return check(mkfifo(pathname.c_str(), S_IRUSR | S_IWUSR | S_IWGRP), "Failed to create fifo");
}

server::server(const std::string& pathname) : pathname(pathname), sockfd(socket(AF_UNIX, SOCK_STREAM, 0)) {
    check_throw(sockfd, "create socket");
}

server::~server() {
    close(sockfd);
    check(unlink(pathname.c_str()), "Failed to delete socket file descriptor");
}

void server::launch() {
    sockaddr_un addr;
    socklen_t addr_size = sizeof(sockaddr_un);
    memset(&addr, 0, sizeof(sockaddr_un));

    addr.sun_family = AF_UNIX;
    std::strcpy(addr.sun_path, pathname.c_str());

    check_throw(bind(sockfd, reinterpret_cast<sockaddr*>(&addr), addr_size), "bind");
    check_throw(listen(sockfd, BACKLOG), "listen");

    std::srand(time(nullptr));
    while (true) {
        sockaddr_un client;
        socklen_t csize;

        int receivedfd = accept(sockfd, reinterpret_cast<sockaddr*>(&client), &csize);
        if (!valid(receivedfd)) {
            perror("Failed to accept");
            close(receivedfd);
            continue;
        }

        umask(0);
        std::string name;
        name = name + "/tmp/fifo_" + std::to_string(time(nullptr)) + "_" + std::to_string(rand()) + "_";
        std::string name_in = name + "in";
        std::string name_out = name + "out";

        if (!makefifo(name_in)) {
            close(receivedfd);
            continue;
        }

        if (!makefifo(name_out)) {
            close(receivedfd);
            unlink(name_in.c_str());
            continue;
        }

        name = name_in + "\\" + name_out;
        size_t size = name.size();
        if (send(receivedfd, name.c_str(), size, 0) != size) {
            perror("Failed to send");
            close(receivedfd);
            continue;
        }
        close(receivedfd);

        int infd = open(name_in.c_str(), O_WRONLY);
        int outfd = open(name_out.c_str(), O_RDONLY);
        while (true) {
            char buffer[BUFFER_SIZE] = {};
            read(outfd, buffer, BUFFER_SIZE);

            std::string message(buffer);
            write(infd, message.c_str(), message.size());

            if (message == "close") {
                close(infd);
                close(outfd);
                unlink(name_in.c_str());
                unlink(name_out.c_str());
                break;
            }
        }
    }
}


} /* ipc_communication_service */
