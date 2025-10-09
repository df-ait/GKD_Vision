# GKD_Vision 🚀

- todo:
  - yolo模型分类器可能还有一些问题， 可能还需要后端进行适配。
  - trackqueueV4中状态转移方程没有改
  - 提升精度。
  - 误差预测
  - 删了好多有用的东西，，。。

## ✨ 特性

- 🎯 **精准目标检测** - 基于 OpenVINO 的 YOLOv7 模型实现高性能装甲板识别
- 🎯 **多目标识别** - 支持英雄、步兵、哨兵等多类目标的精确分类
- 🏃 **动态跟踪** - 使用卡尔曼滤波器实现平滑的目标跟踪
- 🎯 **PnP 解算** - 实现精确的 3D-2D 坐标转换和弹道补偿
- 🤖 **多兵种支持** - 支持英雄、步兵、哨兵等不同机器人类型
- 📸 **相机支持** - 集成海康威视相机驱动（HIK driver）
- ⚡ **高性能** - 优化的多线程处理架构

## 🛠️ 依赖项

- **visionlib Framework** - 核心视觉框架
- **OpenCV 4.5.4+** - 计算机视觉处理
- **OpenVINO 2024.6.0** - 模型推理加速
- **Eigen3** - 线性代数计算
- **Ceres Solver** - 优化求解器
- **海康威视 SDK** - 相机驱动支持
- **C++20** - 编译要求

## 📋 项目结构

```
GKD_Vision/
├── main.cpp              # 主程序入口
├── include/              # 头文件
│   ├── attack/           # 攻击策略模块
│   ├── data_manager/     # 数据管理模块
│   ├── garage/           # 目标管理模块
│   ├── kalman/           # 卡尔曼滤波模块
│   ├── model/            # 模型推理模块
│   ├── pointer/          # 指向模块
│   ├── solver/           # 计算求解模块
│   ├── structure/        # 数据结构定义
│   ├── threads/          # 多线程处理
│   ├── timer/            # 定时器
│   ├── utils/            # 工具函数
│   ├── video/            # 视频处理
│   └── visionlib.h       # 总体头文件
├── src/                  # 源代码
├── config/               # 配置文件
├── libs/                 # 第三方库
└── run.sh                # 构建运行脚本
```

## 🚀 快速开始

### 构建与运行

```bash
# 进入项目目录
cd /path/to/GKD_Vision

# 构建并运行（指定机器人类型）
sudo ./run.sh infantry -s      # 步兵模式，带图像显示
sudo ./run.sh hero -v          # 英雄模式，详细输出
sudo ./run.sh sentry_l         # 左哨兵模式
sudo ./run.sh sentry_r         # 右哨兵模式
```

### 命令行参数

- `infantry` | `hero` | `sentry_l` | `sentry_r` - 机器人类型
- `-r` - 重新构建项目
- `-s` - 启用图像显示
- `-v` - 启用详细输出
- `-c` - 更新配置文件
- `-g "message"` - Git 提交并推送

## ⚙️ 配置说明

配置文件位于 `config/[robot_type]/config.yaml`，主要参数包括：

- `model_path_xml` / `model_path_bin` - OpenVINO 模型路径
- `conf_threshold` - 检测置信度阈值
- `cam_gain` - 相机增益
- `cam_exptime` - 相机曝光时间
- `framerate` - 相机帧率
- `shoot_speed` - 弹丸初速度
- `armor_small_h/w` - 小装甲板尺寸
- `armor_large_h/w` - 大装甲板尺寸

## 🎯 主要模块

### Attack 模块 💥
- 智能攻击策略
- 目标优先级管理
- ID 识别修正机制

### Kalman 滤波 📊
- 目标状态预测
- 平滑跟踪轨迹
- 减少噪声干扰

### Solver 解算 🔢
- PnP 位姿解算
- 弹道补偿计算
- 3D 坐标转换

### Video 处理 📹
- 海康相机驱动
- 图像采集与预处理
- 实时视频流处理
