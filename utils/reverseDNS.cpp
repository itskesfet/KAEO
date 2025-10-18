/*
To extrace hop router names.
*/
#include "../include/reverseDNS.hpp"

#include <netdb.h>
#include <arpa/inet.h>

std::string resolve_hostname(const std::string& ip) {
    struct sockaddr_in sa;
    sa.sin_family = AF_INET;
    inet_pton(AF_INET, ip.c_str(), &sa.sin_addr);

    char host[NI_MAXHOST];
    if (getnameinfo((struct sockaddr*)&sa, sizeof(sa), host, sizeof(host), nullptr, 0, 0) == 0) {
        if (host==ip) {
            return "Unknown.";
        }
        return std::string(host);
    } else {
        return "Unable to extract."; 
    }
}
