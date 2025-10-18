#pragma once
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/udp.h>   
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <netinet/ip_icmp.h>
#define MAX_TTL 65
#define id_ 18
uint16_t checksum(void* vdata, size_t length);
void build_packet(char* packet, const char* src_ip, const char* dst_ip , int ttl_, int pid_);