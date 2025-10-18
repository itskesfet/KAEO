#pragma once
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
struct address_net{
    bool isipv4;
    bool isipv6;
    std::string ipv4;
    std::string ipv6;
    address_net():isipv4(false),isipv6(false),ipv4(""),ipv6(""){}
};
void resolveTarget(const std::string& target, address_net& netaddr);