#pragma once
#include "../include/resolveTarget.hpp"
#include <sys/epoll.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <chrono>
struct ProbeInfo { uint16_t ttl; uint16_t seq; uint64_t send_ts; };
#include "../include/handleReply.hpp"
void communicate(int sockfd, address_net& netaddr, std::vector<HopInfo>& vhops, std::unordered_map<uint64_t, ProbeInfo>& sent_probes,std::vector<ProbeInfo>& sent_probe_info);
inline uint64_t encode_key(uint16_t id, uint16_t seq) {
    return (static_cast<uint64_t>(id) << 16) | seq;
}
inline uint64_t timestamp() {
    using namespace std::chrono;
    return duration_cast<microseconds>(
               steady_clock::now().time_since_epoch()
           ).count();  
}
