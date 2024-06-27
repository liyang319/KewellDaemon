#include "CmdDispatcher.h"
#include <iostream>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "Base.h"
#include "Utility.h"
#include "Base.h"

// using namespace rapidjson;

CmdDispatcher::CmdDispatcher(const std::string &json_data)
{
    // COUT<< "---"<< json_data <<endl;
    m_document.Parse(json_data.c_str());

    if (m_document.HasParseError())
    {
        std::cerr << "Failed to parse JSON data" << endl;
    }
}

void CmdDispatcher::DispatchCommand()
{
    if (m_document.HasMember("cmd"))
    {
        std::string cmd = m_document["cmd"].GetString();
        if (cmd == CTRL_CMD_REPORT_IP)
        {
            ProcessReportIPCmd();
        }
        else if (cmd == CTRL_CMD_SET_IP)
        {
            ProcessSetIPCmd();
        }
        else if (cmd == CTRL_CMD_NET_RESET)
        {
            ProcessNetResetCmd();
        }
        else
        {
            std::cerr << "Unknown command: " << cmd << endl;
        }
    }
    else
    {
        std::cerr << "Command not found in JSON" << endl;
    }
}

void CmdDispatcher::ProcessReportIPCmd()
{
    COUT << "Processing ReportIP command" << endl;
    rapidjson::Document doc;
    doc.SetObject();
    rapidjson::Value cmd;
    cmd.SetString("ReportIP_ack");
    doc.AddMember("cmd", cmd, doc.GetAllocator());

    // std::string strIP = Utility::getSystemIP("ens33");
    // std::string strMac = Utility::getSystemMac("ens33");

    // rapidjson::Value ip;
    // ip.SetString(strIP.c_str(), strIP.length(), doc.GetAllocator());
    // rapidjson::Value mac;
    // mac.SetString(strMac.c_str(), strMac.length(), doc.GetAllocator());

    // doc.AddMember("IP", ip, doc.GetAllocator());
    // doc.AddMember("MAC", mac, doc.GetAllocator());

    // rapidjson::StringBuffer buffer;
    // rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    // doc.Accept(writer);
    // COUT << buffer.GetString() << endl;

    // UpperportData data;
    // std::memcpy(data.content.data(), buffer.GetString(), std::min(buffer.GetSize(), data.content.size()));
    // data.len = std::min(buffer.GetSize(), data.content.size());
    // AppData::getInstance().addDataToCtrlSendQueue(data);
}

void CmdDispatcher::ProcessSetIPCmd()
{
    COUT << "Processing SetIP command" << endl;
    std::string striface = "";
    if (m_document.HasMember("iface"))
    {
        striface = m_document["iface"].GetString();
    }
    std::string strMASK = "";
    if (m_document.HasMember("MASK"))
    {
        strMASK = m_document["MASK"].GetString();
    }
    std::string strGATEWAY = "";
    if (m_document.HasMember("GATEWAY"))
    {
        strGATEWAY = m_document["GATEWAY"].GetString();
    }
    std::string strDNS = "";
    if (m_document.HasMember("DNS"))
    {
        strDNS = m_document["DNS"].GetString();
    }
    std::string strIP = "";
    if (m_document.HasMember("IP"))
    {
        strIP = m_document["IP"].GetString();
    }
    std::string strMAC = "";
    if (m_document.HasMember("MAC"))
    {
        strMAC = m_document["MAC"].GetString();
    }
    // Utility::setNetworkConfig(striface, strMASK, strGATEWAY, strDNS, strIP, strMAC); // network config

    // rapidjson::Document doc;
    // doc.SetObject();
    // rapidjson::Value cmd;
    // cmd.SetString("SetIP_ack");
    // doc.AddMember("cmd", cmd, doc.GetAllocator());

    // rapidjson::Value act;
    // std::string strAck = "Setting network";
    // act.SetString(strAck.c_str(), strAck.length(), doc.GetAllocator());
    // doc.AddMember("act", act, doc.GetAllocator());

    // rapidjson::StringBuffer buffer;
    // rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    // doc.Accept(writer);
    // COUT << buffer.GetString() << endl;

    // UpperportData data;
    // std::memcpy(data.content.data(), buffer.GetString(), std::min(buffer.GetSize(), data.content.size()));
    // data.len = std::min(buffer.GetSize(), data.content.size());
    // AppData::getInstance().addDataToCtrlSendQueue(data);
}

void CmdDispatcher::ProcessNetResetCmd()
{
    COUT << "Processing NetReset command" << endl;

    // Utility::restartNetwork(); // restrt network

    // rapidjson::Document doc;
    // doc.SetObject();
    // rapidjson::Value cmd;
    // cmd.SetString("NetReset_ack");
    // doc.AddMember("cmd", cmd, doc.GetAllocator());

    // rapidjson::Value act;
    // std::string strAck = "Restarting network";
    // act.SetString(strAck.c_str(), strAck.length(), doc.GetAllocator());
    // doc.AddMember("act", act, doc.GetAllocator());

    // rapidjson::StringBuffer buffer;
    // rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    // doc.Accept(writer);
    // COUT << buffer.GetString() << endl;

    // UpperportData data;
    // std::memcpy(data.content.data(), buffer.GetString(), std::min(buffer.GetSize(), data.content.size()));
    // data.len = std::min(buffer.GetSize(), data.content.size());
    // AppData::getInstance().addDataToCtrlSendQueue(data);
}
