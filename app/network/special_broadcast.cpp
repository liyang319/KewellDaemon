#include "special_broadcast.hpp"
#include "file_log.hpp"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <net/if.h>
#include <netinet/in.h>
#include <sstream>
#include <string>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

SpecBcst::SpecBcst(/* args */)
{
    // 创建UDP套接字
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        log_msg(ERROR, "Error creating socket");
    }

    // 开启SO_REUSEADDR选项，允许端口被重用
    int reuse = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char *)&reuse, sizeof(reuse)) < 0) {
        log_msg(ERROR, "Error setting socket options");
    }

    // 设置套接字为非阻塞模式
    int flags = fcntl(sockfd, F_GETFL, 0);
    if (flags == -1) {
        log_msg(ERROR, "Error: Failed to get socket flags\n");
        close(sockfd);
        return;
    }
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

    // 设置广播地址结构
    memset(&broadcastAddr, 0, sizeof(broadcastAddr));
    broadcastAddr.sin_family = AF_INET;
    broadcastAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    broadcastAddr.sin_port = htons(PORT);

    // 将套接字与广播地址绑定
    if (bind(sockfd, (struct sockaddr *)&broadcastAddr, sizeof(broadcastAddr)) < 0) {
        log_msg(ERROR, "Error binding socket to broadcast address");
    }
}

SpecBcst::~SpecBcst() { close(sockfd); }

// 获取本机IP地址
std::string SpecBcst::getIPAddress()
{
    int fd;
    struct ifreq ifr;

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        log_msg(ERROR, "Error: Failed to create socket\n");
        exit(EXIT_FAILURE);
    }

    strncpy(ifr.ifr_name, "eth0", IFNAMSIZ - 1);

    if (ioctl(fd, SIOCGIFADDR, &ifr) < 0) {
        log_msg(ERROR, "Error: ioctl failed to get IP address\n");
        close(fd);
        exit(EXIT_FAILURE);
    }

    close(fd);

    return inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);
}

// 获取本机MAC地址
std::string SpecBcst::getMACAddress()
{
    int fd;
    struct ifreq ifr;

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        std::cerr << "Error: Failed to create socket\n";
        exit(EXIT_FAILURE);
    }

    strncpy(ifr.ifr_name, "eth0", IFNAMSIZ - 1);

    if (ioctl(fd, SIOCGIFHWADDR, &ifr) < 0) {
        std::cerr << "Error: ioctl failed to get MAC address\n";
        close(fd);
        exit(EXIT_FAILURE);
    }

    close(fd);

    char macAddress[18] = {0};
    sprintf(macAddress, "%02X:%02X:%02X:%02X:%02X:%02X",
            (unsigned char)ifr.ifr_hwaddr.sa_data[0],
            (unsigned char)ifr.ifr_hwaddr.sa_data[1],
            (unsigned char)ifr.ifr_hwaddr.sa_data[2],
            (unsigned char)ifr.ifr_hwaddr.sa_data[3],
            (unsigned char)ifr.ifr_hwaddr.sa_data[4],
            (unsigned char)ifr.ifr_hwaddr.sa_data[5]);

    return macAddress;
}

//修改系统文件实现IP地址的切换
void SpecBcst::modifyNetworkConfig(const std::string &configFile, const std::string &interface, const std::string &ip, const std::string &subnet, const std::string &gateway, const std::string &dns, const std::string &mac)
{
    std::ifstream inFile(configFile); //打开输入文件
    std::stringstream strStream;      //stringstream流

    // Read the file into stringstream
    strStream << inFile.rdbuf();
    std::string fileContent = strStream.str();

    // Find the position of the interface in the file
    size_t pos = fileContent.find("iface " + interface);

    if (pos != std::string::npos) {
        // Modify IP address
        pos = fileContent.find("address ", pos);
        if (pos != std::string::npos) {
            pos += std::string("address ").length();
            size_t endPos = fileContent.find("\n", pos);
            fileContent.replace(pos, endPos - pos, ip);
        }

        // Modify Subnet Mask
        pos = fileContent.find("netmask ", pos);
        if (pos != std::string::npos) {
            pos += std::string("netmask ").length();
            size_t endPos = fileContent.find("\n", pos);
            fileContent.replace(pos, endPos - pos, subnet);
        }

        // Modify Gateway
        pos = fileContent.find("gateway ", pos);
        if (pos != std::string::npos) {
            pos += std::string("gateway ").length();
            size_t endPos = fileContent.find("\n", pos);
            fileContent.replace(pos, endPos - pos, gateway);
        }

        // Modify DNS
        pos = fileContent.find("dns-nameservers ", pos);
        if (pos != std::string::npos) {
            pos += std::string("dns-nameservers ").length();
            size_t endPos = fileContent.find("\n", pos);
            fileContent.replace(pos, endPos - pos, dns);
        }

        // Modify MAC address
        pos = fileContent.find("hwaddress ether", pos);
        if (pos != std::string::npos) {
            pos += std::string("hwaddress ether").length();
            size_t endPos = fileContent.find("\n", pos);
            fileContent.replace(pos, endPos - pos, mac);
        }

        // Write the modified content back to the file
        std::ofstream outFile(configFile);
        outFile << fileContent;
        outFile.close();

        log_msg(DEBUG, "Network configuration updated successfully.");
    } else {
        log_msg(DEBUG, "Interface not found in the configuration file.");
    }
}

bool SpecBcst::restartNetworkService()
{
    // 重启网络服务的命令
    std::string command = "sudo systemctl restart networking";

    // 执行命令
    int result = system(command.c_str());
    if (result != 0) {
        std::cerr << "Failed to restart network service." << std::endl;
        return false;
    }

    return true;
}

void SpecBcst::spec_bcst_ack()
{
    char recvBuffer[MAX_BUFFER_SIZE];
    int numBytesReceived;
    socklen_t client_addr_len = sizeof(client_addr);

    // 接收数据包
    numBytesReceived = recvfrom(sockfd, recvBuffer, MAX_BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &client_addr_len);
    if (numBytesReceived == 0) {
        // 对端已关闭连接
    } else if (numBytesReceived == -1) {
        if (errno == EWOULDBLOCK || errno == EAGAIN) {
            // 没有数据可用
        } else {
            log_msg(DEBUG, "Error: Failed to receive data\n");
        }
    } else {
        recvBuffer[numBytesReceived] = 0x00;
        //有可用数据按照json格式解析
        rapidjson::Document doc;
        doc.SetObject();
        rapidjson::Document::AllocatorType &allocator = doc.GetAllocator(); //生成一个分配器
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        if (!doc.Parse(recvBuffer).HasParseError()) { //通过HasParseError来判断解析是否成功
            if (doc.HasMember("cmd")) {
                if (doc["cmd"] == "ReportIP") { //要求上报地址命令
                    if (doc.HasMember("IP")) {
                        rapidjson::Value &IP = doc["IP"];        //找到节点
                        std::string ip_str = getIPAddress();     //节点值
                        IP.SetString(ip_str.c_str(), allocator); //修改节点值
                    } else {
                        rapidjson::Value strNode(rapidjson::kStringType); //新建一个节点
                        std::string ip_str = getIPAddress();              //节点值
                        strNode.SetString(ip_str.c_str(), allocator);
                        doc.AddMember("IP", strNode, allocator); //添加节点
                    }
                    if (doc.HasMember("MAC")) {
                        rapidjson::Value &MAC = doc["MAC"];
                        std::string mac_str = getMACAddress();
                        MAC.SetString(mac_str.c_str(), allocator);
                    } else {
                        rapidjson::Value str_node(rapidjson::kStringType);
                        std::string mac_str = getMACAddress();
                        str_node.SetString(mac_str.c_str(), allocator);
                        doc.AddMember("MAC", str_node, allocator);
                    }
                    doc.Accept(writer); //将DOM stringfy 为json
                    std::string responseMessage = buffer.GetString();
                    sendto(sockfd, responseMessage.c_str(), responseMessage.length(), 0, (struct sockaddr *)&client_addr, client_addr_len);
                } else if (doc["cmd"] == "SetIP") { //要求设置地址命令,修改网络配置
                    std::string interface = "eth0";
                    std::string ipAddress = "192.168.1.100";
                    std::string macAddress = "00:11:22:33:44:55";
                    std::string subnetMask = "255.255.255.0";
                    std::string gateway = "192.168.1.1";
                    std::string dns = "8.8.8.8";
                    if (doc.HasMember("iface")) {
                        interface = doc["iface"].GetString();
                    }
                    if (doc.HasMember("IP")) {
                        ipAddress = doc["IP"].GetString();
                    }
                    if (doc.HasMember("MAC")) {
                        macAddress = doc["MAC"].GetString();
                    }
                    if (doc.HasMember("MASK")) {
                        subnetMask = doc["MASK"].GetString();
                    }
                    if (doc.HasMember("GATEWAY")) {
                        gateway = doc["GATEWAY"].GetString();
                    }
                    if (doc.HasMember("DNS")) {
                        dns = doc["DNS"].GetString();
                    }
                    std::string responseMessage = "Setting network!";
                    sendto(sockfd, responseMessage.c_str(), responseMessage.length(), 0, (struct sockaddr *)&client_addr, client_addr_len);
                    modifyNetworkConfig(IPCONFIGFILE, interface, ipAddress, subnetMask, gateway, dns, macAddress);
                } else if (doc["cmd"] == "NetReset") { //要求复位网络
                    std::string responseMessage = "Restarting network!";
                    sendto(sockfd, responseMessage.c_str(), responseMessage.length(), 0, (struct sockaddr *)&client_addr, client_addr_len);
                    restartNetworkService();
                }
            }
        }
    }
}
