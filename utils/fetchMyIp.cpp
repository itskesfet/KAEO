/*
To fetch my IP.
*/
#include "../include/fethMyIp.hpp"
#include <cstddef>
#include <iostream>

std::string get_my_ip() {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        std::cout << "Socket creation failed.\n";
        return "NULL"; 
    }

    sockaddr_in serv{};
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr("8.8.8.8"); 
    serv.sin_port = htons(53); 


    if (connect(sock, (sockaddr*)&serv, sizeof(serv)) == -1) {
        std::cout << "Connect failed: " << "\n";
        return NULL; 
    }

    sockaddr_in name{};
    socklen_t namelen = sizeof(name);

    if (getsockname(sock, (sockaddr*)&name, &namelen) == -1) { // ✨ Check for error
        std::cout << "getsockname failed.\n";
        close(sock);
        return "NULL";
    }
    
    char buffer[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &name.sin_addr, buffer, sizeof(buffer));

    close(sock);
    return std::string(buffer);
}
