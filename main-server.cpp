#include <iostream>

#include "service/server.hpp"
#include "service/service-exception.hpp"

namespace service = ipc_communication_service;


int main(int argc, char const* argv[]) {
    std::string pathname(argc == 2 ? argv[1] : "/tmp/socket");

    try {
        service::server zs(pathname);
        zs.launch();
    } catch (const service::service_exception& e) {
        std::cout << e.what() << '\n';
        return 1;
    }
    return 0;
}
