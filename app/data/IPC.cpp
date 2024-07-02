#include "IPC.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <cstring>

IPC::IPC()
{
    mq_id = -1;
}

IPC::~IPC()
{
    if (mq_id != -1)
    {
        close_msgqueue();
    }
}

void IPC::open_msgqueue()
{
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 1024;
    attr.mq_curmsgs = 0;

    mq_id = mq_open("/my_mq", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, &attr);
    if (mq_id == -1)
    {
        perror("mq_open");
    }
}

void IPC::close_msgqueue()
{
    mq_close(mq_id);
    mq_unlink("/my_mq");
    mq_id = -1;
}

void IPC::send_message(const std::string &message)
{
    if (mq_id == -1)
    {
        open_msgqueue();
    }

    if (mq_send(mq_id, message.c_str(), message.length(), 0) == -1)
    {
        perror("mq_send");
    }
}

std::string IPC::recv_message()
{
    if (mq_id == -1)
    {
        open_msgqueue();
    }

    char buffer[1024];
    unsigned int prio;
    ssize_t bytes_read = mq_receive(mq_id, buffer, 1024, &prio);
    if (bytes_read == -1)
    {
        perror("mq_receive");
        return "";
    }

    buffer[bytes_read] = '\0';
    return std::string(buffer);
}

void IPC::clear_message_queue()
{
    std::string message;
    while (true)
    {
        message = recv_message();
        if (message.empty())
        {
            break;
        }
    }
}
