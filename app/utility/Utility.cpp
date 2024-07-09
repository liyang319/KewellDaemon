#include "Utility.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include "Base.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <netpacket/packet.h>
#include <cstdlib>

#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

std::string Utility::removeTrailingNewline(std::string str)
{
    std::string modifiedStr = str;
    if (!modifiedStr.empty() && modifiedStr.back() == '\n')
    {
        modifiedStr.erase(modifiedStr.length() - 1);
    }
    return modifiedStr;
}

std::string Utility::getFileContent(std::string fileName)
{
    std::ifstream file(fileName);
    std::string content = "";
    std::string line;

    if (file.is_open())
    {
        while (std::getline(file, line))
        {
            content += line;
            if (!file.eof())
            {
                content += "\n";
            }
        }
        file.close();
    }
    else
    {
        std::cerr << "Error opening file: " << fileName << std::endl;
    }

    return content;
}

int Utility::replaceFileWithCmd(std::string orginalFile, std::string newFile)
{
    std::string command = "cp " + newFile + " " + orginalFile;
    int result = system(command.c_str()); // 执行命令行

    if (result == 0)
    {
        return 0; // 成功
    }
    else
    {
        return -1; // 失败
    }
}

bool Utility::isFileEmpty(std::string filename)
{
    std::ifstream file(filename, std::ios::binary | std::ios::ate); // 打开文件并移动文件指针到文件末尾
    if (file.is_open())
    {
        std::streampos size = file.tellg(); // 获取文件指针位置，即文件大小
        file.close();                       // 关闭文件流

        return size == 0;
    }
    else
    {
        std::cout << "无法打开文件" << std::endl;
        return false;
    }
}

int replaceFile(const std::string orginalFile, const std::string newFile)
{
    std::ifstream newFileInput(newFile);
    if (!newFileInput)
    {
        std::cerr << "Error: Unable to open new file" << std::endl;
        return -1;
    }

    std::ofstream orginalFileOutput(orginalFile);
    if (!orginalFileOutput)
    {
        std::cerr << "Error: Unable to open orginal file" << std::endl;
        return -1;
    }

    orginalFileOutput << newFileInput.rdbuf();

    return 0;
}

int Utility::runFile(std::string executablePath, bool bBackground)
{
    // std::string command = "./" + executablePath; // 构建启动可执行文件的命令行
    // int result = system(command.c_str());        // 执行命令行
    // return result;
    std::string command = executablePath; // 使用绝对路径启动可执行文件
    if (bBackground)
        command += " &";
    int result = system(command.c_str()); // 执行命令行
    return result;
}

int Utility::killApp(std::string processName)
{
    // std::string processName = "example_process";     // 要杀死的进程名字
    std::string command = "pkill -f " + processName; // 构造要执行的命令
    int result = system(command.c_str());            // 执行命令
    if (result == 0)
    {
        COUT << "进程 " << processName << " 已成功被杀死。" << std::endl;
    }
    else
    {
        std::cerr << "无法杀死进程 " << processName << "。" << std::endl;
    }
    return result;
}

int Utility::changeFileMode(std::string filename, std::string mode)
{
    std::string command = "chmod " + mode + " " + filename;
    COUT << "---cmd = " << command << std::endl;
    int result = system(command.c_str());
    if (result == 0)
    {
        COUT << "文件权限修改成功" << std::endl;
    }
    else
    {
        std::cerr << "文件权限修改失败" << std::endl;
    }
    return result;
}

// std::string Utility::calculateMD5(std::string filename)
// {
//     std::ifstream file(filename, std::ios::binary);
//     if (!file)
//     {
//         std::cerr << "Cannot open file: " << filename << std::endl;
//         return "";
//     }

//     MD5_CTX ctx;
//     MD5_Init(&ctx);

//     char buffer[1024];
//     while (file)
//     {
//         file.read(buffer, sizeof(buffer));
//         std::streamsize count = file.gcount();
//         if (count > 0)
//         {
//             MD5_Update(&ctx, buffer, count);
//         }
//     }

//     unsigned char md[MD5_DIGEST_LENGTH];
//     MD5_Final(md, &ctx);

//     std::stringstream ss;
//     for (int i = 0; i < MD5_DIGEST_LENGTH; ++i)
//     {
//         ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(md[i]);
//     }

//     return ss.str();
// }

void Utility::fillVersionFile(std::string filename, std::string content)
{
    // Open the file in out mode to clear the original content
    std::ofstream file(filename, std::ios::out | std::ios::trunc);

    if (file.is_open())
    {
        // Write the new content to the file
        file << content;

        // Close the file
        file.close();

        COUT << "Content written to " << filename << " successfully." << std::endl;
    }
    else
    {
        std::cerr << "Error opening file " << filename << std::endl;
    }
}

int Utility::unzipFile(std::string zipFileName, std::string outputDirectory)
{
    std::string command = "unzip " + zipFileName + " -d " + outputDirectory;
    COUT << "-----unzipFile-----" << command << endl;
    int result = system(command.c_str());
    // if (result != 0)
    // {
    //     std::cerr << "Error: Failed to unzip file " << zipFileName << std::endl;
    // }
    return result;
}

std::string Utility::getFilenameFromUrl(std::string url)
{
    size_t found = url.find_last_of("/\\");
    if (found != std::string::npos)
    {
        return url.substr(found + 1);
    }
    else
    {
        return "";
    }
}

void Utility::deleteFiles(std::string dir)
{
    DIR *dp = opendir(dir.c_str());
    if (dp != NULL)
    {
        struct dirent *ep;
        while ((ep = readdir(dp)))
        {
            if (ep->d_type == DT_REG)
            { // regular file
                std::string filePath = dir + "/" + ep->d_name;
                if (remove(filePath.c_str()) != 0)
                {
                    std::cerr << "Failed to delete file: " << filePath << std::endl;
                }
            }
        }
        closedir(dp);
    }
    else
    {
        std::cerr << "Failed to open directory: " << dir << std::endl;
    }
}

bool Utility::deleteDirectory(std::string dir)
{
    DIR *dp = opendir(dir.c_str());
    if (dp != NULL)
    {
        struct dirent *ep;
        bool success = true;
        while ((ep = readdir(dp)))
        {
            if (ep->d_type == DT_DIR)
            { // directory
                if (strcmp(ep->d_name, ".") != 0 && strcmp(ep->d_name, "..") != 0)
                {
                    std::string subDirPath = dir + "/" + ep->d_name;
                    if (!deleteDirectory(subDirPath))
                    {
                        success = false;
                    }
                }
            }
            else if (ep->d_type == DT_REG)
            { // regular file
                std::string filePath = dir + "/" + ep->d_name;
                if (remove(filePath.c_str()) != 0)
                {
                    std::cerr << "Failed to delete file: " << filePath << std::endl;
                    success = false;
                }
            }
        }
        closedir(dp);
        if (rmdir(dir.c_str()) != 0)
        {
            std::cerr << "Failed to delete directory: " << dir << std::endl;
            success = false;
        }
        return success;
    }
    else
    {
        std::cerr << "Failed to open directory: " << dir << std::endl;
        return false;
    }
}

bool Utility::createDirIfNotExist(std::string dirPath)
{
    struct stat st;
    if (stat(dirPath.c_str(), &st) != 0)
    {
        if (mkdir(dirPath.c_str(), 0777) == 0)
        {
            return true;
        }
        else
        {
            std::cerr << "Failed to create directory: " << dirPath << std::endl;
            return false;
        }
    }
    else
    {
        if (S_ISDIR(st.st_mode))
        {
            COUT << "Directory already exists: " << dirPath << std::endl;
            return true;
        }
        else
        {
            std::cerr << dirPath << " is not a directory" << std::endl;
            return false;
        }
    }
}

bool Utility::copyFileTo(std::string filePath, std::string dstPath)
{
    std::ifstream srcFile(filePath, std::ios::binary);
    if (!srcFile)
    {
        std::cerr << "Failed to open source file: " << filePath << std::endl;
        return false;
    }

    std::string dstFilePath = dstPath + "/" + filePath.substr(filePath.find_last_of('/') + 1);
    std::ofstream dstFile(dstFilePath, std::ios::binary);
    if (!dstFile)
    {
        srcFile.close();
        std::cerr << "Failed to create destination file: " << dstFilePath << std::endl;
        return false;
    }
    dstFile << srcFile.rdbuf();

    srcFile.close();
    dstFile.close();

    return true;
}

bool Utility::fileExists(std::string fileName)
{
    std::ifstream file(fileName);
    return file.good();
}

bool Utility::isExecutable(std::string fileName)
{
    return access(fileName.c_str(), X_OK) == 0;
}

bool Utility::startApp(std::string executablePath, bool bBackground)
{
    if (!fileExists(executablePath))
    {
        COUT << "File " << executablePath << " does not exist" << std::endl;
        return false;
    }
    if (!isExecutable(executablePath))
    {
        COUT << "File " << executablePath << " is not executable" << std::endl;
        return false;
    }
    std::string command = executablePath; // 使用绝对路径启动可执行文件
    if (bBackground)
        command += " &";
    int result = system(command.c_str());
    if (result == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Utility::FeedWatchDog()
{
    std::cout << "-----------FeedWatchDog----------" << endl;
    system("echo 1 > /dev/watchdog");
}

void Utility::CloseWatchDog()
{
    std::cout << "-----------CloseWatchDog----------" << endl;
    system("echo V > /dev/watchdog");
}
/// @brief
/// @param interfaceName
/// @return /
std::string Utility::getSystemIP(const std::string &interfaceName)
{
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0)
    {
        perror("socket failed");
        return "";
    }

    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, interfaceName.c_str(), IFNAMSIZ);

    struct sockaddr_in *addr = reinterpret_cast<struct sockaddr_in *>(&ifr.ifr_addr);
    addr->sin_family = AF_INET;

    if (ioctl(fd, SIOCGIFADDR, &ifr) == 0)
    {
        close(fd);
        return std::string(inet_ntoa(addr->sin_addr));
    }
    close(fd);
    return "";
}

std::string Utility::getSystemMac(const std::string &interfaceName)
{
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0)
    {
        perror("socket failed");
        return "";
    }

    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, interfaceName.c_str(), IFNAMSIZ);

    struct sockaddr_ll addr;
    if (ioctl(fd, SIOCGIFHWADDR, &ifr) == 0)
    {
        close(fd);
        char mac[18];
        snprintf(mac, sizeof(mac), "%02x:%02x:%02x:%02x:%02x:%02x",
                 (unsigned char)ifr.ifr_hwaddr.sa_data[0],
                 (unsigned char)ifr.ifr_hwaddr.sa_data[1],
                 (unsigned char)ifr.ifr_hwaddr.sa_data[2],
                 (unsigned char)ifr.ifr_hwaddr.sa_data[3],
                 (unsigned char)ifr.ifr_hwaddr.sa_data[4],
                 (unsigned char)ifr.ifr_hwaddr.sa_data[5]);
        return std::string(mac);
    }

    close(fd);
    return "";
}

void Utility::setNetworkConfig(std::string iface, std::string mask, std::string gateway, std::string dns, std::string ip,
                               std::string mac)
{
    std::string command = "ifconfig " + iface + " " + ip + " netmask " + mask + " hw ether " + mac;

    // Set IP address, netmask and MAC address
    system(command.c_str());

    // Set default gateway
    command = "route add default gw " + gateway;
    system(command.c_str());

    // Set DNS server
    command = "echo \"nameserver " + dns + "\" > /etc/resolv.conf";
    system(command.c_str());
}

// 修改系统文件实现IP地址的切换
void Utility::modifyNetworkConfig(const std::string &configFile, const std::string &interface, const std::string &ip, const std::string &subnet, const std::string &gateway, const std::string &dns, const std::string &mac)
{
    std::ifstream inFile(configFile); // 打开输入文件
    std::stringstream strStream;      // stringstream流

    // Read the file into stringstream
    strStream << inFile.rdbuf();
    std::string fileContent = strStream.str();

    // Find the position of the interface in the file
    size_t pos = fileContent.find("iface " + interface);

    if (pos != std::string::npos)
    {
        // Modify IP address
        pos = fileContent.find("address ", pos);
        if (pos != std::string::npos)
        {
            pos += std::string("address ").length();
            size_t endPos = fileContent.find("\n", pos);
            fileContent.replace(pos, endPos - pos, ip);
        }

        // Modify Subnet Mask
        pos = fileContent.find("netmask ", pos);
        if (pos != std::string::npos)
        {
            pos += std::string("netmask ").length();
            size_t endPos = fileContent.find("\n", pos);
            fileContent.replace(pos, endPos - pos, subnet);
        }

        // Modify Gateway
        pos = fileContent.find("gateway ", pos);
        if (pos != std::string::npos)
        {
            pos += std::string("gateway ").length();
            size_t endPos = fileContent.find("\n", pos);
            fileContent.replace(pos, endPos - pos, gateway);
        }

        // Modify DNS
        pos = fileContent.find("dns-nameservers ", pos);
        if (pos != std::string::npos)
        {
            pos += std::string("dns-nameservers ").length();
            size_t endPos = fileContent.find("\n", pos);
            fileContent.replace(pos, endPos - pos, dns);
        }

        // Modify MAC address
        pos = fileContent.find("hwaddress ether", pos);
        if (pos != std::string::npos)
        {
            pos += std::string("hwaddress ether").length();
            size_t endPos = fileContent.find("\n", pos);
            fileContent.replace(pos, endPos - pos, mac);
        }

        // Write the modified content back to the file
        std::ofstream outFile(configFile);
        outFile << fileContent;
        outFile.close();

        std::cout << "Network configuration updated successfully." << std::endl;
    }
    else
    {
        std::cout << "Interface not found in the configuration file." << std::endl;
    }
}

bool Utility::restartNetwork()
{
    // 执行重启网络的命令
    int result = system("sudo systemctl restart network");

    // 检查命令执行结果
    if (result == 0)
    {
        std::cout << "Network has been restarted successfully." << std::endl;
        return true;
    }
    else
    {
        std::cout << "Failed to restart network." << std::endl;
        return false;
    }
}