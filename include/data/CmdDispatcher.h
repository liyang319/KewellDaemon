#ifndef CMD_DISPATCHER_H
#define CMD_DISPATCHER_H

#include "rapidjson/document.h"
#include <string>
#include "UpperBroadcastReceiver.h"

#define CTRL_CMD_REPORT_IP "ReportIP"
#define CTRL_CMD_SET_IP "SetIP"
#define CTRL_CMD_NET_RESET "NetReset"

class CmdDispatcher
{
public:
    CmdDispatcher(const std::string &json_data, UpperBroadcastReceiver *pReceiver);

    void DispatchCommand();

private:
    void ProcessReportIPCmd();

    void ProcessSetIPCmd();

    void ProcessNetResetCmd();

    rapidjson::Document m_document;
    UpperBroadcastReceiver *pBroadCastReceiver;
};

#endif // CMD_DISPATCHER_H
