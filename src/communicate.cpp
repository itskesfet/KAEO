#include "../include/communicate.hpp"
#include "../include/handleReply.hpp"
#include "../include/constructPacket.hpp"
#include "../include/handleReply.hpp"
#include "../include/fethMyIp.hpp"

#include <chrono>
#include <unistd.h>

#define MAX_EVENTS 100
#define timeout_ms 5000
#define PROBES_PER_TTL 3


void communicate(int sockfd, address_net& netaddr , std::vector<HopInfo>& vhops , std::unordered_map<uint64_t, ProbeInfo>& sent_probes, std::vector<ProbeInfo>& sent_probe_info){
    
    fcntl(sockfd, F_SETFL, O_NONBLOCK);
    int epfd = epoll_create1(EPOLL_CLOEXEC);
    struct epoll_event ev;
    ev.events = EPOLLIN;        
    ev.data.fd = sockfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);

    int ttl=1;
    int probe_index = 0;
    uint16_t id = getpid() & 0xFFFF;
    (void) id; //messed it up
    std::string myip = get_my_ip();

    for (ttl = 1; ttl <= MAX_TTL; ++ttl) {
        for (probe_index = 0; probe_index < PROBES_PER_TTL; ++probe_index) {

                char packet[sizeof(struct iphdr)+sizeof(struct icmphdr)];
                build_packet(packet, myip.c_str() , netaddr.ipv4.c_str() , ttl , probe_index);
                int pid_ = getpid();
                uint64_t key = encode_key(pid_, probe_index);

                ProbeInfo info;
                info.ttl = ttl;
                info.seq = probe_index;
                info.send_ts = timestamp();
                sent_probes[key] = info;
                sent_probe_info.push_back(info);

                struct sockaddr_in dest{};
                dest.sin_family = AF_INET;
                dest.sin_port = htons(0); 
                inet_pton(AF_INET, netaddr.ipv4.c_str(), &dest.sin_addr);
                
                int sendtoint = sendto(sockfd, packet, sizeof(packet), 0, (struct sockaddr*)&dest, sizeof(dest));
                if (sendtoint>0) {
                    //in next update

                }
        }
    }


    struct epoll_event events[MAX_EVENTS];
    auto start = std::chrono::steady_clock::now();

    while (true) { 
        int n = epoll_wait(epfd, events, MAX_EVENTS, 100); 
        if (n > 0) {
            for (int i = 0; i < n; ++i) {
                if (events[i].data.fd == sockfd && (events[i].events & EPOLLIN)) {
                    char buf[4096];
                    ssize_t len = recvfrom(sockfd, buf, sizeof(buf), 0, nullptr, nullptr);
                    if (len > 0) {
                        read_reply_(buf, sent_probes , vhops);
                    }
                }
            }
        }

        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count() > timeout_ms)
            break;
    }

}