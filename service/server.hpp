#ifndef ZYGOTE_SERVER_HPP
#define ZYGOTE_SERVER_HPP

#include <string>
#include "socket.hpp"

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
    usocket sc;
    static constexpr size_t BUFFER_SIZE = 1024; // reading buffer size
};

} /* ipc_communication_service */

#endif /* end of include guard: ZYGOTE_SERVER_HPP */
