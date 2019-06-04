#include <string>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "client.hpp"
#include "service-exception.hpp"
#include "utils.hpp"

namespace ipc_communication_service {

client::client(const std::string& pathname) {
    int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    check_throw(sockfd, "create socket");

    sockaddr_un addr;
    const socklen_t addr_size = sizeof(sockaddr_un);
    memset(&addr, 0, addr_size);

    addr.sun_family = AF_UNIX;
    std::strcpy(addr.sun_path, pathname.c_str());

    check_throw(connect(sockfd, reinterpret_cast<sockaddr*>(&addr), addr_size), "connect");

    char buffer[BUFFER_SIZE] = {};
    if (!valid(read(sockfd, buffer, BUFFER_SIZE))) {
        close(sockfd);
        throw service_exception("read");
    }
    close(sockfd);

    std::string strbuffer(buffer);
    std::string::size_type boundary = strbuffer.find("\\");

    infd = open(strbuffer.substr(0, boundary).c_str(), O_RDONLY);
    outfd = open(strbuffer.substr(boundary + 1).c_str(), O_WRONLY);
}

client::~client() {
    yell("close");
    close(infd);
    close(outfd);
}

std::string client::yell(const std::string& str) {
    write(outfd, str.c_str(), str.size());
    char buffer[BUFFER_SIZE] = {};
    read(infd, buffer, BUFFER_SIZE);
    return buffer;
}

} /* ipc_communication_service */
