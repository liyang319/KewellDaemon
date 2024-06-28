#include "Daemon.hpp"
#include "IPC.hpp"
#include "file_log.hpp"
// #include "heart_beat.hpp"
// #include "special_broadcast.hpp"
#include <cstdlib> // 用于 system 函数
#include <fcntl.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include "Base.h"
#include "Utility.h"

Daemon::Daemon()
{
    no_running_count = 0;
    no_alive_count = 0;
}

Daemon::~Daemon()
{
}

void Daemon::run()
{
    // daemonize(); // 进入守护进程模式
    // broadcastReceiver.Start();

    while (true)
    {
        checkMainProcess(); // 检查数据中心状态
        usleep(500000);     // 500毫秒
    }
}

void Daemon::daemonize()
{
    // 创建子进程，父进程退出，子进程成为孤儿进程
    pid_t pid = fork();

    if (pid < 0)
    {
        // 创建子进程失败
        COUT << "Failed to fork." << std::endl;
        exit(1);
    }

    if (pid > 0)
    {
        // 父进程退出
        exit(0);
    }

    // 在新的会话中启动子进程
    if (setsid() < 0)
    {
        COUT << "Failed to create new session." << std::endl;
        exit(1);
    }

    // 将工作目录切换到根目录
    if (chdir("/") < 0)
    {
        COUT << "Failed to change working directory to root." << std::endl;
        exit(1);
    }
}

void Daemon::checkMainProcess()
{
    // COUT << "----checkMainProcess----" << std::endl;
    if (!isMainProcessAlive())
    {
        // COUT << "-------No main process-------" << std::endl;
        // stopProcess(PROCESS_NAME);
        Utility::killApp(PROCESS_NAME);
        // 再启动“数据中心”这个程序
        Utility::runFile(PROCESS_FULL_PATH, true);
        // log_msg(DEBUG, "center is not running, restart it.");
    }
}

void Daemon::poweroff()
{
    // 执行linux的POWEROFF指令
    if (execlp("sudo", "sudo", "poweroff", NULL) == -1)
    {
        // execlp()执行失败
        COUT << "Failed to execute poweroff command." << std::endl;
    }
}

void Daemon::reboot()
{
    // 执行linux的REBOOT指令
    if (execlp("sudo", "sudo", "reboot", NULL) == -1)
    {
        // execlp()执行失败
        COUT << "Failed to execute reboot command." << std::endl;
    }
}

// 检查某一可执行文件名的程序是否在运行
bool checkProcessRunning(const std::string &processName)
{
    std::string command = "pgrep -x " + processName + " > /dev/null 2>&1"; // 不输出到终端
    int result = system(command.c_str());
    return result == 0;
}

// 检测“数据中心”程序是否在运行
bool Daemon::isMainProcessAlive()
{
    if (checkProcessRunning(PROCESS_NAME))
    {
        no_running_count = 0;
    }
    else
    {
        no_running_count++;
    }
    if (no_running_count >= 20)
    {
        no_running_count = 0;
        return false;
    }
    return true;
}