#pragma once

#include <arpa/inet.h>
#include <netinet/ip.h>
#include <unordered_map>
#include <unordered_map>     
#include <cstdint>            
#include <netinet/ip.h>       
#include <netinet/ip_icmp.h>  
#include <arpa/inet.h>        
#include <cstring>  
#include <vector>
#include <string>
#include <numeric>
struct HopInfo {
    std::string ip;
    std::vector<double> rtts; 
    double avg_rtt = 0.0;  
        void update_avg_rtt() {
        avg_rtt = !rtts.empty() 
                  ? std::accumulate(rtts.begin(), rtts.end(), 0.0) / rtts.size()
                  : 0.0;
    }
};
#include "../include/communicate.hpp"
void read_reply_(const char* buf,std::unordered_map<uint64_t, ProbeInfo>& sent_probes , std::vector<HopInfo>& vhops);