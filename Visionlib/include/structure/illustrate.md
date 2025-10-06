没什么可以说的， 放一个ai的说明吧...

# Structure模块 - 活跃使用的数据结构

本目录包含视觉系统中活跃使用的数据结构和基础算法模块，是整个系统的基础架构。

## 1. enums.hpp - 枚举类型定义

### 功能
定义系统中使用的各种枚举类型，提供类型安全和代码可读性。

### 主要枚举
- **ArmorColor**: 装甲板颜色 (`ARMOR_COLOR_BLUE`, `ARMOR_COLOR_RED`, `ARMOR_COLOR_NONE`, `ARMOR_COLOR_PURPLE`)
- **ArmorSize**: 装甲板大小 (`ARMOR_SIZE_UNKNOWN`, `ARMOR_SIZE_SMALL_ARMOR`, `ARMOR_SIZE_BIG_ARMOR`)
- **ArmorID**: 装甲板ID (`ARMOR_ID_SENTRY`, `ARMOR_ID_HERO`, `ARMOR_ID_INFANTRY_3/4/5`, `ARMOR_ID_TOWER`等)
- **Config**: 通信配置位掩码（用于串口通信协议）

### 使用场景
- 机器人识别和分类
- 颜色识别和处理
- 串口通信协议

## 2. stamp.hpp - 核心数据结构

### 功能
定义系统中所有重要数据结构，用于模块间数据传递。

### 主要结构
- **Frame**: 帧数据结构
  - `image`: 图像数据
  - `time_point`: 时间戳
  - `yolo_list`: YOLO检测结果
  - `armor_list`: 装甲板列表
  - `target_list`: 目标列表

- **Armor**: 装甲板数据
  - `id`, `color`, `size`: 基本属性
  - `four_points`: 四个角点坐标
  - `center`, `rect`: 中心点和矩形框

- **YoloRect**: YOLO检测结果
  - `four_points`: 四点坐标
  - `box`: 矩形边界框
  - `confidence`: 置信度

- **Target**: 跟踪目标
  - `pose_world`: 世界坐标
  - `armor_yaw_world`: 装甲板朝向
  - `armor_id`: 装甲板ID

### 使用场景
- 图像处理流水线
- 目标检测和跟踪
- 数据传递和存储

## 3. slidestd.hpp - 滤波平滑算法

### 功能
提供滑动窗口统计算法，用于数据平滑和噪声过滤。

### 主要类
- **SlideStd<T>**: 滑动标准差计算器
  - `push(value)`: 添加新值到队列
  - `getStd()`: 获取标准差
  - `getVar()`: 获取方差
  - `getAvg()`: 获取平均值

- **SlideAvg<T>**: 滑动平均计算器
  - `push(value)`: 添加新值到队列
  - `getAvg()`: 获取平均值

### 使用场景
- 跟踪数据平滑
- 噪声过滤
- 稳定性分析

## 4. cyclequeue.hpp - 循环队列

### 功能
提供固定大小的循环队列，用于存储历史数据。

### 主要类
- **CycleQueue<T>**: 循环队列
  - `push(value)`: 添加新值（超出容量时自动移除最老的值）
  - `pop()`: 获取队首元素
  - `getAvg()`: 获取队列平均值
  - `getSum()`: 获取队列总和

### 使用场景
- 延迟计算（delay_list）
- 历史数据存储
- 平均值计算

## 5. speedqueue.hpp - 速度队列

### 功能
专门用于弹速计算的队列，支持加权平均。

### 主要类
- **SpeedQueue<T>**: 速度队列
  - 支持加权计算
  - 用于弹速估计

### 主要用途
- 弹速计算
- 发射参数调整

## 整体架构
这些数据结构构成了整个视觉系统的数据基础，提供了：
- **类型安全**: 通过枚举避免魔法数字
- **数据传递**: 统一的数据结构定义
- **算法支持**: 基础统计和队列算法
- **性能优化**: 滑动窗口避免重复计算