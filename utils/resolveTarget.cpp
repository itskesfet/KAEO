/*
To extract target ip 
args -  target ip and the address_net struct to store the result.
*/
#include "../include/resolveTarget.hpp"

#include <cstddef>
#include <iostream>
#include <sys/socket.h>

void resolveTarget(const std::string& target, address_net& netaddr){

    
    struct addrinfo hints{};
    struct addrinfo *servinfo , *p ;
    hints.ai_family = AF_UNSPEC;
    hints.ai_flags = AI_CANONNAME;
    hints.ai_socktype = SOCK_RAW;
    
    int status;
    char ipstr[INET6_ADDRSTRLEN]; 
    
    if ((status = getaddrinfo(target.c_str(), NULL, &hints, &servinfo)) != 0) {
        std::cout << "getaddrinfo error: " << gai_strerror(status) << "\n";
        exit(1);
    }


    for (p = servinfo; p != nullptr; p = p->ai_next) {

        void* addr = nullptr;
        std::string ipvs;

        if (p->ai_family == AF_INET) {
            struct sockaddr_in* ipv4 = (struct sockaddr_in*)p->ai_addr;
            addr = &(ipv4->sin_addr);
            ipvs = "IPv4";
            netaddr.isipv4=true;
            inet_ntop(AF_INET, addr, ipstr, sizeof(ipstr));
            netaddr.ipv4 = ipstr;
        } else { 
            struct sockaddr_in6* ipv6 = (struct sockaddr_in6*)p->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipvs = "IPv6";
            netaddr.isipv6 = true;
            inet_ntop(AF_INET6, addr, ipstr, sizeof(ipstr));
            netaddr.ipv6 = ipstr;

        }
    }

freeaddrinfo(servinfo);
return ;
}
