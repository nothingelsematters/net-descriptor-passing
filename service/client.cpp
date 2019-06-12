#include <string>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "client.hpp"
#include "socket.hpp"
#include "service-exception.hpp"
#include "utils.hpp"

namespace ipc_communication_service {

client::client(const std::string& pathname) : fd(path_readfd(pathname)) {}

int client::path_readfd(const std::string& pathname) {
    usocket sc;
    sc.connect(pathname);
    return readfd(sc.sockfd);
}

client::~client() {
    close(fd);
}

std::string client::yell(const std::string& str) {
    send_message(fd, str);
    char buffer[BUFFER_SIZE] = {};
    read(fd, buffer, BUFFER_SIZE);
    return std::string(buffer);
}

} /* ipc_communication_service */
