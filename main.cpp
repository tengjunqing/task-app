#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <array>
#include <stdexcept>
#include <unistd.h>  // 添加 sleep 函数

class SimpleSystemMonitor {
public:
    // 获取15分钟平均负载 - 直接解析文件
    double getCPULoad15() {
        std::ifstream file("/proc/loadavg");
        if (!file.is_open()) {
            throw std::runtime_error("无法打开 /proc/loadavg");
        }
        
        std::string line;
        std::getline(file, line);
        
        std::istringstream iss(line);
        double load1, load5, load15;
        iss >> load1 >> load5 >> load15;
        
        return load15;
    }

    // 获取内存占用率 - 直接解析文件
    double getMemoryUsage() {
        std::ifstream file("/proc/meminfo");
        if (!file.is_open()) {
            throw std::runtime_error("无法打开 /proc/meminfo");
        }
        
        std::string line;
        long total = 0, available = 0;
        
        while (std::getline(file, line)) {
            if (line.find("MemTotal:") == 0) {
                // 提取数字部分，跳过"MemTotal:"和空格
                std::string numStr = line.substr(line.find(":") + 1);
                numStr.erase(0, numStr.find_first_not_of(" ")); // 去除前导空格
                numStr = numStr.substr(0, numStr.find(" ")); // 取第一个单词（数字）
                total = std::stol(numStr);
            } else if (line.find("MemAvailable:") == 0) {
                std::string numStr = line.substr(line.find(":") + 1);
                numStr.erase(0, numStr.find_first_not_of(" "));
                numStr = numStr.substr(0, numStr.find(" "));
                available = std::stol(numStr);
            }
            
            if (total > 0 && available > 0) break;
        }
        
        if (total > 0 && available >= 0) {
            long used = total - available;
            return (static_cast<double>(used) / total) * 100.0;
        }
        
        throw std::runtime_error("无法解析内存信息");
    }

    // 获取存储占用率
    double getStorageUsage() {
        std::string result = executeCommand("df / | tail -1");
        
        // 简单的字符串分割代替正则表达式
        std::istringstream iss(result);
        std::string filesystem, blocks, used, available, percent, mount;
        iss >> filesystem >> blocks >> used >> available >> percent >> mount;
        
        // 去除百分号
        if (!percent.empty() && percent.back() == '%') {
            percent.pop_back();
        }
        
        return std::stod(percent);
    }

    // 获取实时CPU使用率（可选功能）
    double getCurrentCPUUsage() {
        std::ifstream file("/proc/stat");
        if (!file.is_open()) {
            throw std::runtime_error("无法打开 /proc/stat");
        }
        
        std::string line;
        std::getline(file, line); // 读取第一行（总CPU信息）
        
        std::istringstream iss(line);
        std::string cpu;
        long user, nice, system, idle, iowait, irq, softirq;
        iss >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq;
        
        long totalIdle = idle + iowait;
        long totalNonIdle = user + nice + system + irq + softirq;
        long total = totalIdle + totalNonIdle;
        
        return (static_cast<double>(totalNonIdle) / total) * 100.0;
    }

private:
    std::string executeCommand(const std::string& cmd) {
        std::array<char, 128> buffer;
        std::string result;
        
        FILE* pipe = popen(cmd.c_str(), "r");
        if (!pipe) throw std::runtime_error("popen()失败!");
        
        while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
            result += buffer.data();
        }
        
        pclose(pipe);
        return result;
    }
};

// 主函数 - 程序入口点
int main() {
    SimpleSystemMonitor monitor;
    
    std::cout << "=== Linux系统资源监控程序 ===" << std::endl;
    std::cout << "正在收集系统信息..." << std::endl;
    std::cout << std::endl;
    
    try {
        // 获取并显示CPU 15分钟平均负载
        double cpuLoad15 = monitor.getCPULoad15();
        std::cout << "📊 CPU 15分钟平均负载: " << cpuLoad15 << std::endl;
        
        // 获取并显示内存占用率
        double memUsage = monitor.getMemoryUsage();
        std::cout << "💾 内存占用率: " << memUsage << "%" << std::endl;
        
        // 获取并显示存储占用率
        double storageUsage = monitor.getStorageUsage();
        std::cout << "💽 存储占用率: " << storageUsage << "%" << std::endl;
        
        // 可选：显示实时CPU使用率
        double currentCPU = monitor.getCurrentCPUUsage();
        std::cout << "⚡ 实时CPU使用率: " << currentCPU << "%" << std::endl;
        
        std::cout << std::endl;
        std::cout << "✅ 监控信息获取完成！" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "❌ 错误: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}