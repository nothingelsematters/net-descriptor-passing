#include <string>
#include <iostream>

#include "service/client.hpp"
#include "service/service-exception.hpp"

namespace service = ipc_communication_service;


int main(int argc, char const* argv[]) {
    std::string pathname = "/tmp/socket";
    std::string claim = "standard claim";

    switch (argc) {
        case 3: claim = argv[2];
        case 2: pathname = argv[1];
    }

    try {
        service::client cn(pathname);
        for (int i = 1; i < 4; ++i) {
            std::cout << cn.yell(claim + "_" + std::to_string(i)) << '\n';
        }
    } catch (const service::service_exception& e) {
        std::cout << e.what() << '\n';
        return 1;
    }
    return 0;
}
