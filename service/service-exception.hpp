#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <stdexcept>
#include <string.h>

namespace ipc_communication_service {

static const std::string PREFIX = "Failed to ";

class service_exception : public std::runtime_error {
public:
    service_exception(const std::string& what_arg) : std::runtime_error(make_message(what_arg)) {}
    service_exception(const char* what_arg) : std::runtime_error(make_message(what_arg)) {}

private:
    std::string make_message(const std::string& str) {
        return "Failed to " + str + ": " + strerror(errno);
    }
};

} /* ipc_communication_service */


#endif /* end of include guard: EXCEPTIONS_HPP */
