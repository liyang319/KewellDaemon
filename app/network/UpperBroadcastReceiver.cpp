#include "UpperBroadcastReceiver.h"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "CmdDispatcher.h"

#define PORT 12345
#define MAX_BUFFER_SIZE 1024

UpperBroadcastReceiver::UpperBroadcastReceiver(int port)
    : port_(port), sockfd_(-1), running_(false)
{
    memset(&broadcastAddr, 0, sizeof(broadcastAddr));
    broadcastAddr.sin_family = AF_INET;
    broadcastAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    broadcastAddr.sin_port = htons(port_);
    index = 0;
}

UpperBroadcastReceiver::~UpperBroadcastReceiver()
{
    Stop();
}

void UpperBroadcastReceiver::Start()
{
    if (running_)
        return;
    // 创建UDP套接字
    if ((sockfd_ = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        // log_msg(ERROR, "Error creating socket");
    }

    // 开启SO_REUSEADDR选项，允许端口被重用
    int reuse = 1;
    if (setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, (const char *)&reuse, sizeof(reuse)) < 0)
    {
        // log_msg(ERROR, "Error setting socket options");
    }

    // 设置套接字为非阻塞模式
    int flags = fcntl(sockfd_, F_GETFL, 0);
    if (flags == -1)
    {
        // log_msg(ERROR, "Error: Failed to get socket flags\n");
        close(sockfd_);
        return;
    }
    fcntl(sockfd_, F_SETFL, flags | O_NONBLOCK);

    // 设置广播地址结构
    memset(&broadcastAddr, 0, sizeof(broadcastAddr));
    broadcastAddr.sin_family = AF_INET;
    broadcastAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    broadcastAddr.sin_port = htons(PORT);

    // 将套接字与广播地址绑定
    if (bind(sockfd_, (struct sockaddr *)&broadcastAddr, sizeof(broadcastAddr)) < 0)
    {
        ; // log_msg(ERROR, "Error binding socket to broadcast address");
    }

    running_ = true;
    recv_thread_ = std::thread(&UpperBroadcastReceiver::Run, this);
}

void UpperBroadcastReceiver::Stop()
{
    if (!running_)
        return;

    running_ = false;
    if (recv_thread_.joinable())
    {
        recv_thread_.join();
    }
    if (sockfd_ >= 0)
    {
        close(sockfd_);
        sockfd_ = -1;
    }
}

void UpperBroadcastReceiver::Run()
{
    char recvBuffer[MAX_BUFFER_SIZE] = {0};
    int numBytesReceived;
    socklen_t client_addr_len = sizeof(client_addr);

    while (running_)
    {
        numBytesReceived = recvfrom(sockfd_, recvBuffer, MAX_BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &client_addr_len);
        if (numBytesReceived > 0)
        {
            recvBuffer[numBytesReceived] = '\0';
            ProcessBroadcastCmd(recvBuffer);
        }
        sleep(1);
    }
}

void UpperBroadcastReceiver::ProcessBroadcastCmd(const std::string &data)
{
    std::cout << "Received broadcast message: " << data << std::endl;
    std::string ss = "reply--" + std::to_string(index);
    index++;
    std::cout << "reply: " << ss << std::endl;
    sendto(sockfd_, ss.c_str(), ss.length(), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
    CmdDispatcher dispatcher(data);
    dispatcher.DispatchCommand();
}
