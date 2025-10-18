#include "../include/resolveTarget.hpp"
#include "../include/setSock.hpp"
#include "../include/communicate.hpp"
#include "../include/banner.hpp"
#include "../include/handleReply.hpp"
#include "../include/reverseDNS.hpp"

#include <cstddef>
#include <stdexcept>
#include <iostream>
#include <string>
#include <vector>


#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define RESET   "\033[0m"
#define VER "01.0"
constexpr double RTT_FAST = 20.0;
constexpr double RTT_MODERATE = 50.0;
size_t max_display = 3; 

#include <cstdlib>
#include <iomanip> 

void clear_screen() {
#ifdef _WIN32
    system("cls"); 
#else
    system("clear"); 
#endif
}

int main(int argc, char* argv[]){
    try {
        if (geteuid() != 0) {
               std::cout<< "\n " << RED << "[E]" << RESET << "  This program must be run with sudo/root privileges.\n";
               return 1;
        }
        if (argc<2) {
            std::cout<< "\n " << RED << "[E]" << RESET << "  Requires minimum of two commands! ex: " << GREEN <<" sudo ./traceroute <example.com> "
            << RESET <<"\n";
            throw std::runtime_error("Input error.\n");
            return -1;
        }

        //Struct to hold target addressing.
        address_net address;
        std::string target = argv[1];
        resolveTarget(target, address);   
        
        std::vector<HopInfo> vhops;

        std::vector<int> sock = constructSock(target);
        for (auto socket : sock) {
            if (socket<0) {
                std::cout<< "\n " << RED << "[E] " << RESET << "Error while Creating New Socket: " << GREEN << "Check Your Internet Connection. "
                << RESET <<"\n";
            
            }
        
        }


        
        
        clear_screen();
        mybanner();
        std::cout << CYAN << "KAEO — A Network Troubleshooter Program for Tracing Packets Across the Network. \n"<< RESET;
        std::cout << WHITE << "KAEO — A Network Troubleshooter for Tracing Packets Across Networks.\n"
                           <<"It operates like traceroute, sending packets with increasing Time-To-Live (TTL) values.\n"
                           <<"Each router that decrements the TTL to zero returns an ICMP “Time Exceeded” message.\n"
                           <<"By analyzing these replies, KAEO maps the route, measures latency, and detects network delays.\n"
                           << RESET <<"\n";
        std::cout << CYAN << "Version : " << VER  << RESET <<"\n";
        std::cout << std::left;

        for (auto p : sock) {
            std::cout << "\nWorking socket: " << p << " on Port: "<< PORT << "\n";
        }

        std::unordered_map<uint64_t, ProbeInfo> sent_probes;
        std::vector<ProbeInfo> sent_probe_info;
        for (auto x : sock) {
            communicate(x,address, vhops  , sent_probes , sent_probe_info);
        }

        std::cout << "Target : "<< target <<YELLOW << "\n->" << RESET <<"    IPv 4/6 : " << address.ipv4 << " / " << address.ipv6 << "\n";
        std::cout<<YELLOW  "->"<<RESET <<"    Number of hops to the Target: " << vhops.size() << "\n";
        size_t routersec=1;
        for (auto hop: vhops) {
            std::string ip_=hop.ip;
            std::string rtt_color;
            if (hop.avg_rtt < RTT_FAST)          rtt_color = GREEN;
            else if (hop.avg_rtt < RTT_MODERATE) rtt_color = YELLOW;
            else                                 rtt_color = RED;

            hop.update_avg_rtt();
            double avgrtt = hop.avg_rtt;
            std::string rtts_;
            for (size_t i = 0; i < hop.rtts.size() && i < max_display; ++i) {
                rtts_ += std::to_string(hop.rtts[i]);
                if (i != hop.rtts.size() - 1 && i != max_display - 1) rtts_ += " | ";  
            }
            if (hop.rtts.size() > max_display) {
                rtts_ += " | ...";
            }
            std::cout <<YELLOW <<"\n->"<< RESET << std::setw(10) << "    Router IP "<< std::setw(16) << ip_<<"";
            std::cout << YELLOW << "\n--->" << RESET << std::setw(10)<<"  Router Name: "<< resolve_hostname(ip_)
            << YELLOW << "\n   >" << RESET 
            << "  RTT: " << rtts_ 
            << YELLOW << "\n   >" << RESET   
            <<"  Seq: "<< routersec  <<" | Avg RTT: "<< rtt_color << avgrtt<<"ms."<<RESET<<"\n";
            routersec++;
            if (static_cast<std::size_t>(routersec)!=vhops.size()) {
                std::cout << "";
            }
        }
        std::cout << "\n"<< RED << "[+]" << RESET << YELLOW << "Execution completed successfully. Return code: 0." << RESET <<" \n";


    } catch (const std::runtime_error& e) {
        std::cerr << "Runtime error occurred: " << e.what() << std::endl;

    }

    return 0;
}