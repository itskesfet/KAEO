#pragma once
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string>
#include <vector>
#define PORT 54321
std::vector<int> constructSock(const std::string& target);