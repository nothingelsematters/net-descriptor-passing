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

server::server(const std::string& pathname) : pathname(pathname) {}

server::~server() {
    check(unlink(pathname.c_str()), "Failed to delete socket file descriptor");
}

void server::launch() {
    sc.listen(pathname);
    std::srand(time(nullptr));

    while (true) {
        sockaddr_un client;
        socklen_t csize;

        int receivedfd = sc.accept(reinterpret_cast<sockaddr*>(&client), csize);
        if (!valid(receivedfd)) {
            perror("Failed to accept");
            close(receivedfd);
            continue;
        }

        std::string name = "/tmp/socket_" + std::to_string(time(nullptr)) + "_" + std::to_string(rand());
        usocket sctmp;
        sctmp.listen(name);
        usocket scout;
        scout.connect(name);

        writefd(receivedfd, scout.sockfd);
        close(receivedfd);

        sockaddr_un tmpclient;
        socklen_t tmpcsize;
        int fdout = sctmp.accept(reinterpret_cast<sockaddr*>(&tmpclient), tmpcsize);

        while (true) {
            char buffer[BUFFER_SIZE] = {};
            read(fdout, buffer, BUFFER_SIZE);
            std::string message(buffer);
            send_message(fdout, message, false);

            if (message == "close") {
                check(unlink(name.c_str()), "Failed to delete socket file descriptor");
                break;
            }
        }
    }
}


} /* ipc_communication_service */
