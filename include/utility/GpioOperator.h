#ifndef GPIOOPERATOR_H
#define GPIOOPERATOR_H

#include <string>

#define DEFAULT_GPIO 123

class GpioOperator
{
public:
    GpioOperator();
    GpioOperator(int gpioPin);
    ~GpioOperator();

    bool initialize();           // 初始化GPIO
    bool readValue();            // 读取GPIO值
    bool writeValue(bool value); // 写入GPIO值
    bool unexport();             // 取消导出GPIO
    bool waitForEdge();

private:
    int m_gpioPin;
    std::string m_gpioPath;
};

#endif
