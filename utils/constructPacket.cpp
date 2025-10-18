/*
Building the ICMP packet 
args- packet buffer , source ip address , destination ip address , time to live which decreases per packets and index of packet being sent.
*/
#include "../include/constructPacket.hpp"

#include <cstdint>
#include <unistd.h>


uint16_t checksum(void* vdata, size_t length) {
    char* data = static_cast<char*>(vdata);
    uint64_t acc = 0xffff;

    for (size_t i = 0; i + 1 < length; i += 2) {
        uint16_t word;
        memcpy(&word, data + i, 2);
        acc += ntohs(word);
        if (acc > 0xffff) acc -= 0xffff;
    }

    if (length & 1) {
        uint16_t word = 0;
        memcpy(&word, data + length - 1, 1);
        acc += ntohs(word);
        if (acc > 0xffff) acc -= 0xffff;
    }
    return htons(~acc);
}


void build_packet(char* packet, const char* src_ip, const char* dst_ip , int ttl_, int index){ 
    uint8_t ttl = static_cast<uint8_t>(ttl_);

    struct iphdr* iph = (struct iphdr*)packet;
    struct icmphdr* icmph = (struct icmphdr*)(packet + sizeof(struct iphdr));

    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = htons(sizeof(struct iphdr) + sizeof(struct icmphdr));
    iph->id = htons(54321);
    iph->protocol = IPPROTO_ICMP;

    iph->ttl = ttl;    
    iph->saddr = inet_addr(src_ip);
    iph->daddr = inet_addr(dst_ip);
    

    icmph->type = ICMP_ECHO;
    icmph->code = 0;
    int pid_ = getpid();
    icmph->un.echo.id = htons(pid_);
    icmph->un.echo.sequence = htons(index);

    iph->check = checksum((unsigned short*)iph, sizeof(struct iphdr));
    icmph->checksum = checksum((unsigned short*)icmph, sizeof(struct icmphdr));

    return ;
}