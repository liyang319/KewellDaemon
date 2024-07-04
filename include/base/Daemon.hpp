#pragma once

#include "IPC.h"
#include <string>
#include <mqueue.h>
#include "UpperBroadcastReceiver.h"

#define PROCESS_NAME "KewellMidware"
#define PROCESS_FULL_PATH "./KewellMidware"
#include "GpioOperator.h"

class Daemon
{
public:
    Daemon();
    ~Daemon();

    void run();

private:
    void daemonize();
    void checkMainProcess();
    void poweroff();
    void reboot();

    bool isMainProcessAlive();
    // bool is_data_center_active();

    // IPC ipc;
    int loopIndex;
    UpperBroadcastReceiver broadcastReceiver;

    // mqd_t mq; // 消息队列描述符
    int32_t no_alive_count, no_running_count;
    GpioOperator gpio;
    bool bPowerSupply;
};
