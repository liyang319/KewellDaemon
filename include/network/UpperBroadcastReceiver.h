#ifndef UPPER_BROADCAST_RECEIVER_H
#define UPPER_BROADCAST_RECEIVER_H

#include <thread>
#include <atomic>
#include <functional>
#include <netinet/in.h>

#define BROADCAST_PORT 12345

class UpperBroadcastReceiver
{
public:
    UpperBroadcastReceiver(int port);
    UpperBroadcastReceiver();
    ~UpperBroadcastReceiver();

    void Start();
    void Stop();

    void ProcessBroadcastCmd(const std::string &data);
    struct sockaddr_in broadcastAddr, client_addr;
    int sockfd_;

private:
    void Run();
    int index;

    int port_;

    std::atomic<bool> running_;
    std::thread recv_thread_;
    // struct sockaddr_in broadcast_addr_;
};

#endif // UPPER_BROADCAST_RECEIVER_H
