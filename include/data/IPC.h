#ifndef IPC_H
#define IPC_H

#include <string>

class IPC
{
public:
    static IPC &getInstance()
    {
        static IPC instance;
        return instance;
    }

    void open_msgqueue();
    void close_msgqueue();
    void send_message(const std::string &message);
    std::string recv_message();
    void clear_message_queue();

private:
    IPC();
    ~IPC();

    int mq_id;
};

#endif // IPC_H
