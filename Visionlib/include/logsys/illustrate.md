原本是uniterm来进行的输出， 但是由于参数面板这个玩意已经没了， 所以我就自己实现一个log系统， 这个log系统会以不同的颜色输出不同的信息，，，

下面是这个log系统的使用方法（我vibe coding出来的）

## 概述
这是一个美观的终端日志输出系统，用于替换原有的参数面板系统。该系统提供彩色终端输出、时间戳、格式化显示等功能，所有输出直接显示在终端上，无需额外的参数面板程序。

## 特性
- ✅ **彩色输出**：不同类型日志使用不同颜色，便于识别
- ✅ **时间戳**：精确到毫秒的时间标记
- ✅ **格式化显示**：参数名称对齐，输出整齐美观
- ✅ **兼容原有接口**：无需修改现有代码
- ✅ **终端直接输出**：无需参数面板程序

## 输出格式
```
[14:30:25.123] [PARAM] pnp dist             : 5.234
[14:30:25.124] [PARAM] target omg           : 0.845  
[14:30:25.125] [WARN ] Tracker timeout
[14:30:25.126] [ERROR] Camera init failed
```

## 颜色说明
- **绿色**：参数信息 (PARAM)
- **红色**：错误信息 (ERROR) 
- **黄色**：警告信息 (WARN)
- **青色**：时间戳
- **白色**：普通信息

## 使用方法

### 1. 基本日志输出
```cpp
#include <uniterm/uniterm.h>

// 输出数值参数
rm::message("pnp dist", 5.234);           // 浮点数
rm::message("target_id", 3);              // 整数
rm::message("yaw_status", 'Y');           // 字符

// 输出消息
rm::message("System initialized");        // 普通消息
rm::message("Connection failed", rm::MSG_ERROR);     // 错误消息
rm::message("Low battery", rm::MSG_WARNING);         // 警告消息
```

### 2. 图像相关日志
```cpp
// 输出矩形框信息
cv::Rect rect(100, 200, 50, 30);
rm::message("detected_armor", 1280, 720, rect);

// 输出四点信息
std::vector<cv::Point2f> points = {{10,10}, {20,10}, {20,20}, {10,20}};
rm::message("four_points", 1280, 720, points);

// 输出点位置
cv::Point2f point(300, 400);
rm::message("center_point", 1280, 720, point);
```

### 3. 系统初始化
```cpp
// 在程序开始时调用（如main.cpp中）
rm::message_init("autoaim");  // 传入系统名称
```

### 4. 信息同步
```cpp
// 在需要同步信息时调用（如控制线程中）
rm::message_send();  // 用于保持接口兼容性
```

## 日志类型
- `rm::MSG_NOTE` - 普通信息（白色）
- `rm::MSG_OK` - 成功信息（绿色）
- `rm::MSG_WARNING` - 警告信息（黄色）
- `rm::MSG_ERROR` - 错误信息（红色）

## 与原系统的兼容性
- 所有 `rm::message` 调用无需修改
- 原有接口完全保持兼容
- 输出格式改为美观的终端显示
- 不再需要共享内存和参数面板

## 编译配置
系统已集成到OpenRM库中，编译时会自动包含。

## 优势
- 立即可视化：日志直接在终端显示
- 调试方便：无需启动额外的参数面板程序
- 性能更好：减少共享内存和进程间通信开销
- 美观清晰：彩色格式便于快速识别问题