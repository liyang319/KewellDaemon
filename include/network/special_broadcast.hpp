#pragma once

#include <netinet/in.h>
#include <string>

#define PORT 9527
#define MAX_BUFFER_SIZE 1024
#define IPCONFIGFILE "/etc/network/interfaces"

class SpecBcst
{
private:
    /* data */
    int sockfd;
    struct sockaddr_in broadcastAddr, client_addr;
    std::string getIPAddress();
    std::string getMACAddress();
    void modifyNetworkConfig(const std::string &configFile, const std::string &interface, const std::string &ip, const std::string &subnet, const std::string &gateway, const std::string &dns, const std::string &mac);
    bool restartNetworkService();

public:
    SpecBcst(/* args */);
    ~SpecBcst();
    void spec_bcst_ack(); //特定广播包内容返回
};
