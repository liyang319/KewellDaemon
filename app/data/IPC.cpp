#include "IPC.hpp"
#include "file_log.hpp"
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <mqueue.h>
#include <signal.h>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include "Base.h"

IPC::IPC() // 初始化文件描述符为无效值
{
    open_msgqueue();
}

IPC::~IPC()
{
    close_msgqueue();
    mq_unlink(QUEUE_NAME);
}

bool IPC::open_msgqueue()
{
    // 打开消息队列
    mq = mq_open(QUEUE_NAME, O_RDWR | O_CREAT, 0666, NULL);
    if (mq == (mqd_t)-1)
    {
        COUT << "open msgqueue ok." << std::endl;
        return false;
    }
    COUT << "open msgqueue ok." << std::endl;
    return true;
}

void IPC::close_msgqueue()
{
    // 关闭已打开的消息队列（如果存在）
    mqd_t mq = mq_open(QUEUE_NAME, O_RDONLY);
    if (mq != (mqd_t)-1)
    {
        mq_close(mq);
    }
    log_msg(NORMAL, "close msgqueue ok.");
}

// 向IPC管道写入消息
void IPC::send_message(const std::string &message)
{
    if (mq_send(mq, message.c_str(), message.size() + 1, 0) == -1)
    {
        log_msg(ERROR, "send message.");
    }
}

// 从IPC管道读取消息
std::string IPC::recv_message()
{
    my_message onemsg;
    std::string message;

    // 尝试接收消息，非阻塞方式
    ssize_t bytes_read = mq_receive(mq, (char *)&onemsg, sizeof(onemsg), NULL);
    if (bytes_read >= 0)
    {
        message.append(onemsg.text, bytes_read);
    }
    else if (errno != EAGAIN)
    {
        // 出现异常
        log_msg(ERROR, "read pipe error!");
        // 关闭管道
        close_msgqueue();
        // 重新打开管道
        open_msgqueue();
    }

    return message;
}
