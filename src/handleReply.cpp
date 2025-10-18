#include "../include/handleReply.hpp"
#include <algorithm>


void read_reply_(const char* buf,std::unordered_map<uint64_t, ProbeInfo>& sent_probes , std::vector<HopInfo>& vhops_info) {
   
    const struct iphdr* ip = reinterpret_cast<const struct iphdr*>(buf);
    
    struct in_addr src_addr;
    src_addr.s_addr = ip->saddr;
    char ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &src_addr, ip_str, sizeof(ip_str));
    double rtt_ms;
    uint64_t recv_time;
    const struct icmphdr* icmp = reinterpret_cast<const struct icmphdr*>(buf + ip->ihl * 4);
    if ( icmp->type == ICMP_TIME_EXCEEDED || icmp->type == ICMP_ECHOREPLY) {
        const struct iphdr* inner_ip = reinterpret_cast<const struct iphdr*>(
            buf + ip->ihl * 4 + sizeof(struct icmphdr)
        );
        const struct icmphdr* inner_icmp = reinterpret_cast<const struct icmphdr*>(
            (char*)inner_ip + inner_ip->ihl * 4
        );

        uint16_t id = ntohs(inner_icmp->un.echo.id);
        uint16_t seq = ntohs(inner_icmp->un.echo.sequence);
        uint64_t key = encode_key(id, seq);


        if (sent_probes.find(key) != sent_probes.end()) {
            const ProbeInfo& info = sent_probes[key];
            recv_time = timestamp();
            rtt_ms = (recv_time - info.send_ts) / 1000.0;
                
            
        }
    }

    auto it = std::find_if(vhops_info.begin(), vhops_info.end(), [&](const HopInfo& hop){ return hop.ip == ip_str; });

    if (it == vhops_info.end()) {
        HopInfo hop;
        hop.ip = ip_str;
        hop.rtts.push_back(rtt_ms);
        vhops_info.push_back(hop);
    } else {
        it->rtts.push_back(rtt_ms);
    }

}