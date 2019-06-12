#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>
#include <string>

#include "socket.hpp"
#include "utils.hpp"

namespace ipc_communication_service {

socket::socket(int domain, int type, int protocol) : sockfd(::socket(domain, type, protocol)) {
    check_throw(sockfd, "create socket");
}

void socket::listen(sockaddr* addr, socklen_t len) {
    check_throw(bind(sockfd, addr, len), "bind");
    check_throw(::listen(sockfd, BACKLOG), "listen");
}

void socket::connect(sockaddr* addr, socklen_t len) {
    check_throw(::connect(sockfd, addr, len), "connect");
}

int socket::accept(sockaddr* receiver, socklen_t rsize) {
    return ::accept(sockfd, reinterpret_cast<sockaddr*>(&receiver), &rsize);
}

socket::~socket() {
    close(sockfd);
}

usocket::usocket() : socket(AF_UNIX, SOCK_STREAM, 0) {}

void usocket::listen(const std::string& pathname) {
    auto addr = make_unaddr(pathname);
    socket::listen(reinterpret_cast<sockaddr*>(&addr), UNSIZE);
}

void usocket::connect(const std::string& pathname) {
    auto addr = make_unaddr(pathname);
    socket::connect(reinterpret_cast<sockaddr*>(&addr), UNSIZE);
}

sockaddr_un usocket::make_unaddr(const std::string& pathname) {
    sockaddr_un addr;
    memset(&addr, 0, UNSIZE);
    addr.sun_family = AF_UNIX;
    std::strcpy(addr.sun_path, pathname.c_str());
    return addr;
}

} /* ipc_communication_service */
