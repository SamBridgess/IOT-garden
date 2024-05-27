#pragma once

#include <inttypes.h>
#include <netinet/in.h>
#include <string>

namespace netutils {
    int ipv4_addr (std::string const &addr, uint16_t port, sockaddr_in &out);
}