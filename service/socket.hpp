#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <cstring>

namespace ipc_communication_service {

class socket {
public:
    socket(int domain, int type, int protocol);
    socket(const socket&) = delete;
    socket& operator=(const socket&) = delete;
    ~socket();

    void listen(sockaddr* addr, socklen_t len);
    void connect(sockaddr* addr, socklen_t len);
    int accept(sockaddr* reciever, socklen_t rsize);

    const int sockfd;
private:
    static constexpr int BACKLOG = 5; // max length to which the queue of pending connections may grow
};

class usocket : public socket {
public:
    usocket();
    void listen(const std::string& pathname);
    void connect(const std::string& pathname);

private:
    static sockaddr_un make_unaddr(const std::string& pathname);
    static constexpr socklen_t UNSIZE = sizeof(sockaddr_un);
};

} /* ipc_communication_service */

#endif /* end of include guard: SOCKET_HPP */
