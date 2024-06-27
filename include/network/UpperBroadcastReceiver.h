#ifndef UPPER_BROADCAST_RECEIVER_H
#define UPPER_BROADCAST_RECEIVER_H

#include <thread>
#include <atomic>
#include <functional>
#include <netinet/in.h>

class UpperBroadcastReceiver
{
public:
    UpperBroadcastReceiver(int port);
    ~UpperBroadcastReceiver();

    void Start();
    void Stop();

    void ProcessBroadcastCmd(const std::string &data);

private:
    void Run();
    int index;

    int port_;
    int sockfd_;
    std::atomic<bool> running_;
    std::thread recv_thread_;
    // struct sockaddr_in broadcast_addr_;
    struct sockaddr_in broadcastAddr, client_addr;
};

#endif // UPPER_BROADCAST_RECEIVER_H
