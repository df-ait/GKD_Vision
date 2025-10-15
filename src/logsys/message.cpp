
#include "logsys/message.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <string>
#include <opencv2/opencv.hpp>

// ANSI颜色代码
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m" 
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BOLD    "\033[1m"

namespace rm {

// 辅助函数：获取当前时间
static std::string getCurrentTime() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;
    
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time_t), "%H:%M:%S");
    oss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return oss.str();
}

// 全局变量声明
std::string NumMsgShmKey = "";
std::string StrMsgShmKey = "";
std::string ImgMsgShmKey = "";

size_t NumShmLen = 0;
size_t StrShmLen = 0;
size_t ImgShmLen = 0;

// 美观的日志输出函数
void message(const std::string& name, int msg) {
    // 输出已被禁用
    // std::cout << "[" << CYAN << getCurrentTime() << RESET << "] " 
    //           << "[PARAM] " << GREEN << std::setw(25) << std::left << name 
    //           << RESET << ": " << WHITE << msg << RESET << std::endl;
}

void message(const std::string& name, float msg) {
    // 输出已被禁用
    // std::cout << "[" << CYAN << getCurrentTime() << RESET << "] " 
    //           << "[PARAM] " << GREEN << std::setw(25) << std::left << name 
    //           << RESET << ": " << WHITE << std::fixed << std::setprecision(3) << msg << RESET << std::endl;
}

void message(const std::string& name, double msg) {
    // 输出已被禁用
    // std::cout << "[" << CYAN << getCurrentTime() << RESET << "] " 
    //           << "[PARAM] " << GREEN << std::setw(25) << std::left << name 
    //           << RESET << ": " << WHITE << std::fixed << std::setprecision(3) << msg << RESET << std::endl;
}

void message(const std::string& name, char msg) {
    // 输出已被禁用
    // std::cout << "[" << CYAN << getCurrentTime() << RESET << "] " 
    //           << "[PARAM] " << GREEN << std::setw(25) << std::left << name 
    //           << RESET << ": " << WHITE << msg << RESET << std::endl;
}

void message(const std::string& name, MsgNum msg) {
    // 输出已被禁用
    // std::cout << "[" << CYAN << getCurrentTime() << RESET << "] " 
    //           << "[PARAM] " << GREEN << std::setw(25) << std::left << name 
    //           << RESET << ": " << WHITE << "MsgNum struct" << RESET << std::endl;
}

void message(const std::string& msg, MSG type) {
    // 输出已被禁用
    // std::string time_str = getCurrentTime();
    // std::string type_str;
    // std::string color;
    // 
    // switch(type) {
    //     case MSG_ERROR: 
    //         type_str = "ERROR"; 
    //         color = RED; 
    //         break;
    //     case MSG_WARNING: 
    //         type_str = "WARN "; 
    //         color = YELLOW; 
    //         break;
    //     case MSG_OK: 
    //         type_str = "OK   "; 
    //         color = GREEN; 
    //         break;
    //     default: 
    //         type_str = "INFO "; 
    //         color = WHITE; 
    //         break;
    // }
    // 
    // std::cout << "[" << CYAN << time_str << RESET << "] " 
    //           << "[" << color << type_str << RESET << "] " 
    //           << WHITE << msg << RESET << std::endl;
}

void message(const std::string& info, int img_width, int img_height, cv::Rect rect) {
    // 输出已被禁用
    // std::string time_str = getCurrentTime();
    // std::cout << "[" << CYAN << time_str << RESET << "] " 
    //           << "[IMAGE] " << GREEN << info 
    //           << RESET << " at (" << rect.x << "," << rect.y 
    //           << ") size(" << rect.width << "x" << rect.height << ")" << RESET << std::endl;
}

void message(const std::string& info, int img_width, int img_height, std::vector<cv::Point2f> four_points) {
    // 输出已被禁用
    // std::string time_str = getCurrentTime();
    // std::cout << "[" << CYAN << time_str << RESET << "] " 
    //           << "[FOURPT] " << MAGENTA << info 
    //           << RESET << " - 4 points at [";
    // for(size_t i = 0; i < four_points.size(); i++) {
    //     std::cout << "(" << four_points[i].x << "," << four_points[i].y << ")";
    //     if(i < four_points.size() - 1) std::cout << ", ";
    // }
    // std::cout << "]" << RESET << std::endl;
}

void message(const std::string& info, int img_width, int img_height, cv::Point2f pointX) {
    // 输出已被禁用
    // std::string time_str = getCurrentTime();
    // std::cout << "[" << CYAN << time_str << RESET << "] " 
    //           << "[POINT] " << BLUE << info 
    //           << RESET << " at (" << pointX.x << "," << pointX.y << ")" << RESET << std::endl;
}

void message_init(const std::string& unique_name) {
    // 输出已被禁用
    // 初始化日志系统，实际项目中可能不需要特殊处理
    // 可以输出一条初始化信息
    // std::cout << "[" << CYAN << getCurrentTime() << RESET << "] " 
    //           << "[INFO] " << WHITE << "Log system initialized with name: " << unique_name << RESET << std::endl;
}

void message_send() {
    // 在美观日志系统中，每次message调用都会立即输出，所以不需要专门的send
    // 可以输出同步信息
    // std::cout << "[" << CYAN << getCurrentTime() << RESET << "] " 
    //           << "[INFO] " << WHITE << "Messages synchronized" << RESET << std::endl;
}



}