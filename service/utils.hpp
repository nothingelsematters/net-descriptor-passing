#ifndef UTILS_HPP
#define UTILS_HPP

#include "service-exception.hpp"

namespace ipc_communication_service {

static bool valid(int arg) {
    return arg != -1;
}

static bool check(int arg, const std::string& message) {
    if (!valid(arg)) {
        perror(message.c_str());
        return false;
    }
    return true;
}

static void check_throw(int arg, const std::string& message) {
    if (!valid(arg)) {
        throw service_exception(message);
    }
}

static void send_message(const int sockfd, const std::string& message, const bool error_termination = true) {
    const char* cstr = message.c_str();
    const size_t size = strlen(cstr);
    size_t sent = 0;

    while (sent < size) {
        ssize_t iter_sent = send(sockfd, cstr + sent, size - sent, 0);
        const std::string errormsg = "send";
        if (error_termination) {
            check_throw(iter_sent, errormsg);
        } else {
            check(iter_sent, errormsg);
        }
        sent += static_cast<size_t>(iter_sent);
    }
}


static void writefd(int sockfd, int fd) {
    size_t size = CMSG_SPACE(sizeof(fd));
    char buffer[size] = {};

    iovec iov = { (void*) "", 1 };
    msghdr message = { nullptr, 0, &iov, 1, buffer, size, 0 };

    cmsghdr* header = CMSG_FIRSTHDR(&message);
    header->cmsg_level = SOL_SOCKET;
    header->cmsg_type = SCM_RIGHTS;
    header->cmsg_len = CMSG_LEN(sizeof(fd));
    reinterpret_cast<int*>(CMSG_DATA(header))[0] = fd;

    message.msg_controllen = header->cmsg_len;
    check(sendmsg(sockfd, &message, 0), "write file descriptor");
}

static int readfd(int sockfd) {
    size_t size = CMSG_SPACE(sizeof(int));
    char buffer[size] = {};
    iovec iov = { nullptr, 0 };
    msghdr message = { nullptr, 0, &iov, 1, buffer, size, 0 };

    check(recvmsg(sockfd, &message, 0), "receive");
    return reinterpret_cast<int*>(CMSG_DATA(CMSG_FIRSTHDR(&message)))[0];
}

} /* ipc_communication_service */


#endif /* end of include guard: UTILS_HPP */
