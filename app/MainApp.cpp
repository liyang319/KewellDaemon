#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <chrono>
#include <iostream>
#include <fstream>
#include "Utility.h"
#include "document.h"
#include "stringbuffer.h"
#include "writer.h"
#include <thread>
#include <chrono>
#include <iomanip>
#include <sstream>
#include "Base.h"
#include "Version.h"
#include "UpperBroadcastReceiver.h"
#include "Daemon.hpp"

using namespace rapidjson;
// maeusing namespace std;

int main()
{
    COUT << "=========Kewell Daemon=============" << VERSION << endl;
    // UpperBroadcastReceiver receiver;
    // receiver.Start();
    Daemon daemon;
    daemon.run();
    // 运行一段时间后停止
    while (1)
    {
        std::this_thread::sleep_for(std::chrono::seconds(100));
    }
    // receiver.Stop();
}
