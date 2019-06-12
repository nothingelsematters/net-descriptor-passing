#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

namespace ipc_communication_service {

class client {
public:
    explicit client(const std::string& pathname);
    ~client();

    std::string yell(const std::string& str);

private:
    const int fd;
    static int path_readfd(const std::string& pathname);
    static constexpr size_t BUFFER_SIZE = 1024;
};

} /* ipc_communication_service */


#endif /* end of include guard: CLIENT_HPP */
