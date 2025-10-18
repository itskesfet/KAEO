![Platform](https://img.shields.io/badge/Platform-Linux-green) ![C++17](https://img.shields.io/badge/C%2B%2B-17-blue) ![Open Source](https://img.shields.io/badge/Open%20Source-Yes-brightgreen) ![Command-Line](https://img.shields.io/badge/CLI-Tool-orange)
![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)

# KAE0 — Network Traceroute Tool

###  <p align="center">KAE0 is a network troubleshooting tool that traces the path packets take from your computer to a target host, similar to the classic traceroute. It sends ICMP packets with increasing Time-To-Live (TTL) values and measures the Round-Trip Time (RTT) for each hop. It also resolves router IPs to hostnames when possible and color-codes RTTs for easy visualization.<p> 
**Version:** 01.0

![Traceroute Output](assets/snap.png)


## Sturcture: 
```
    Mtraceroute/
    ├── bin/                  # Compiled binary of the traceroute program
    ├── include/              # Header files declaring functions and structures
    ├── src/                  # Source files implementing functionality
    ├── utils/                # Helper functions and utilities
    ├── kaeo.sh               # Wrapper script to run traceroute with sudo
    └── readme.md             # Project description and usage guide
```
## How to Run:
```
    Open a terminal and navigate to the Mtraceroute folder:
    1> make
    2> ./kaeo.sh <target>
```
## Example Output
```
    -> Router IP: 192.168.1.1
    ---> Router Name: Unknown
       > RTT: 13.899ms | 14.115ms | 14.228ms
       > Seq: 1 | Avg RTT: 14.080ms
``` 
