#ifndef ZYGOTE_SERVER_HPP
#define ZYGOTE_SERVER_HPP

#include <string>

namespace ipc_communication_service {

class server {
public:
    explicit server(const std::string& pathname);

    server(const server&) = delete;
    server& operator=(const server&) = delete;
    ~server();

    void launch();

private:
    const std::string pathname;
    const int sockfd;

    static constexpr int BACKLOG = 5; // max length to which the queue of pending connections may grow
    static constexpr size_t BUFFER_SIZE = 1024; // reading buffer size
};

} /* ipc_communication_service */

#endif /* end of include guard: ZYGOTE_SERVER_HPP */
