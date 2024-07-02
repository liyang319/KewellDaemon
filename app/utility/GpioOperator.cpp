#include "GpioOperator.h"
#include <fstream>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>

GpioOperator::GpioOperator() : m_gpioPin(DEFAULT_GPIO)
{
    m_gpioPath = "/sys/class/gpio/gpio" + std::to_string(m_gpioPin) + "/";
}

GpioOperator::GpioOperator(int gpioPin) : m_gpioPin(gpioPin)
{
    m_gpioPath = "/sys/class/gpio/gpio" + std::to_string(m_gpioPin) + "/";
}

GpioOperator::~GpioOperator()
{
    unexport();
}

bool GpioOperator::initialize()
{
    std::ofstream exportFile("/sys/class/gpio/export");
    if (!exportFile.is_open())
    {
        std::cerr << "Error: Unable to open export file" << std::endl;
        return false;
    }
    exportFile << m_gpioPin;
    exportFile.close();
    return true;
}

bool GpioOperator::readValue()
{
    std::ifstream valueFile(m_gpioPath + "value");
    if (!valueFile.is_open())
    {
        std::cerr << "Error: Unable to open value file" << std::endl;
        return false;
    }
    std::string value;
    valueFile >> value;
    valueFile.close();
    std::cout << "GPIO" << m_gpioPin << " value: " << value << std::endl;
    return true;
}

bool GpioOperator::writeValue(bool value)
{
    std::ofstream valueFile(m_gpioPath + "value");
    if (!valueFile.is_open())
    {
        std::cerr << "Error: Unable to open value file" << std::endl;
        return false;
    }
    valueFile << (value ? "1" : "0");
    valueFile.close();
    return true;
}

bool GpioOperator::unexport()
{
    std::ofstream unexportFile("/sys/class/gpio/unexport");
    if (!unexportFile.is_open())
    {
        std::cerr << "Error: Unable to open unexport file" << std::endl;
        return false;
    }
    unexportFile << m_gpioPin;
    unexportFile.close();
    return true;
}

bool GpioOperator::waitForEdge()
{
    std::string valuePath = m_gpioPath + "value";
    int gpio_fd = open(valuePath.c_str(), O_RDONLY);
    if (gpio_fd == -1)
    {
        std::cerr << "Error: Unable to open GPIO file" << std::endl;
        return false;
    }

    struct pollfd fdset[1];
    fdset[0].fd = gpio_fd;
    fdset[0].events = POLLPRI;

    while (true)
    {
        int ret = poll(fdset, 1, -1);
        if (ret > 0)
        {
            if (fdset[0].revents & POLLPRI)
            {
                lseek(gpio_fd, 0, SEEK_SET); // 将文件指针移回文件开头
                char buf;
                read(gpio_fd, &buf, 1);
                std::cout << "GPIO" << m_gpioPin << " value changed: " << buf << std::endl;
                // 在这里可以添加处理GPIO变化的逻辑
            }
        }
        else
        {
            std::cerr << "Error: poll failed" << std::endl;
            break;
        }
    }

    close(gpio_fd);
    return true;
}
