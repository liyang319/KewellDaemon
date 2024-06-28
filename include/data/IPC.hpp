#ifndef IPC_HPP
#define IPC_HPP

#include <cstring>
#include <iostream>
#include <mqueue.h>
#include <string>

#define QUEUE_NAME "/ICC_queue"

// 结构体定义消息格式
struct my_message {
    int type;
    char text[128]; // 假设消息文本最大长度为 256 字符
};

class IPC
{
public:
    IPC();
    ~IPC();

    void send_message(const std::string &message);
    std::string recv_message();

private:
    bool open_msgqueue();
    void close_msgqueue();
    mqd_t mq; // 消息队列描述符
};

#endif // IPC_HPP
