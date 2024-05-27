#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>

#include <iostream>

#include "netutils.hpp"

namespace netutils {

    int ipv4_addr (std::string const &addr, uint16_t port, sockaddr_in &out)
    {
        struct hostent *host_info;

        host_info = gethostbyname(addr.c_str());

        if ( host_info == NULL ) {
            return 1;
        }

        memset(&out, 0, sizeof(out));
        in_addr *address = (in_addr *) host_info->h_addr_list[0];
        out.sin_addr.s_addr = address->s_addr;
        std::cout << inet_ntoa(out.sin_addr) << std::endl;

        out.sin_port = htons(port);
        out.sin_family = AF_INET;
        return 0;
    }

}  // namespace netutils