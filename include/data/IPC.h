#ifndef IPC_H
#define IPC_H

#include <string>

class IPC
{
public:
    IPC();
    ~IPC();

    void open_msgqueue();
    void close_msgqueue();
    void send_message(const std::string &message);
    std::string recv_message();
    void clear_message_queue();

private:
    int mq_id;
};

#endif // IPC_H
