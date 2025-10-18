/*
Seting the socket for communication and return socket vector.
*/
#include "../include/setSock.hpp"

#include <iostream>

std::vector<int> constructSock(const std::string& target){

    std::vector<int> sock_vec;

    struct addrinfo hints{};
    struct addrinfo *servinfo , *p;
    hints.ai_family = AF_INET;
    hints.ai_protocol = IPPROTO_ICMP;
    hints.ai_socktype = SOCK_RAW;

    int sockfd;
    int status;
    int sockfdopt;

    if ((status = getaddrinfo(target.c_str(), nullptr, &hints, &servinfo)) != 0) {
        std::cout << "getaddrinfo error: " << gai_strerror(status) << "\n";
        std::cout<< "Check your network.\n";
        sock_vec.push_back(-1);

    }

    for (p = servinfo; p != nullptr; p = p->ai_next) {
        int on = 1;

        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockfd == -1) {
            std::cout << "Socket Failed\n";
            continue;
        }

        sockfdopt = setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on));
        if (sockfdopt == -1) {
            std::cout << "setsockopt(IP_HDRINCL) failed \n -1 return by setsockopt.";
            continue;
        }

        if (p->ai_family==AF_INET){
            sock_vec.push_back(sockfd);
        }



    }
    freeaddrinfo(servinfo);

return sock_vec;
}