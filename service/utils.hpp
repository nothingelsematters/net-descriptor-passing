#ifndef UTILS_HPP
#define UTILS_HPP

#include "service-exception.hpp"

namespace ipc_communication_service {

bool valid(int arg) {
    return arg != -1;
}

bool check(int arg, const std::string& message) {
    if (!valid(arg)) {
        perror(message.c_str());
        return false;
    }
    return true;
}

void check_throw(int arg, const std::string& message) {
    if (!valid(arg)) {
        throw service_exception(message);
    }
}

} /* ipc_communication_service */


#endif /* end of include guard: UTILS_HPP */
