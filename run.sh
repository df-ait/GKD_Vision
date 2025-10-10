#!/bin/bash
# ==============================================================
# GKD_Vision 一键构建 & 部署 & 运行脚本
# 支持自动识别兵种并注入 CMake 宏定义
# ==============================================================

blue="\033[1;34m"
yellow="\033[1;33m"
red="\033[1;31m"
reset="\033[0m"

include_count=$(find include -type f \( -name "*.cpp" -o -name "*.h" \) -exec cat {} \; | wc -l)
src_count=$(find src -type f \( -name "*.cpp" -o -name "*.h" -o -name "*.txt" \) -exec cat {} \; | wc -l)
total=$((include_count + src_count))

# ==============================================================
# 0. 系统目录准备
# ==============================================================

export LD_LIBRARY_PATH=$(pwd)/lib/64:$LD_LIBRARY_PATH

SYS_DIR="/etc/visionlib"
if [ ! -d "$SYS_DIR" ]; then
    echo -e "${yellow}创建系统配置目录: $SYS_DIR${reset}"
    sudo install -d -m755 -o root -g root "$SYS_DIR"
fi

if [ ! -d "config" ]; then
    echo -e "${yellow}创建 config -> $SYS_DIR 的软链接${reset}"
    ln -s "$SYS_DIR" ./config
fi

if [ ! -d "build" ]; then
    mkdir build
fi

imshow=0
verbose=0

# ==============================================================
# 1. 参数解析
# ==============================================================

while getopts ":rcg:ls" opt; do
    case $opt in
        r)
            echo -e "${yellow}<--- Clean build directory --->${reset}"
            sudo rm -rf build
            mkdir build
            ;;
        c)
            echo -e "${yellow}<--- Copying uniconfig to $SYS_DIR --->${reset}"
            sudo cp -r data/uniconfig/* "$SYS_DIR"/
            sudo chmod -R 755 "$SYS_DIR"
            exit 0
            ;;
        g)
            git_message=$OPTARG
            echo -e "${yellow}\n<--- Git commit: $git_message --->${reset}"
            git pull
            git add -A
            git commit -m "$git_message"
            git push
            exit 0
            ;;
        l)
            cd ../visionlib || exit 1
            sudo ./run.sh
            cd ../GKD_Vision || exit 1
            exit 0
            ;;
        s)
            imshow=1
            ;;
        \?)
            echo -e "${red}未知参数: -$OPTARG${reset}"
            ;;
        :)
            echo -e "${red}参数 -$OPTARG 需要一个值${reset}"
            ;;
    esac
done
shift $((OPTIND - 1))

# ==============================================================
# 2. 兵种参数识别
# ==============================================================

TARGET="$1"
ARMY_DEFINE=""

case "$TARGET" in
    infantry)
        ARMY_DEFINE="-DTJURM_INFANTRY"
        ;;
    hero)
        ARMY_DEFINE="-DTJURM_HERO"
        ;;
    sentry_l|sentry_r)
        ARMY_DEFINE="-DTJURM_SENTRY"
        ;;
    balance)
        ARMY_DEFINE="-DTJURM_BALANCE"
        ;;
    dronse)
        ARMY_DEFINE="-DTJURM_DRONSE"
        ;;
    *)
        echo -e "${red}❌ 未识别的兵种: $TARGET${reset}"
        echo "用法: ./run.sh [infantry|hero|sentry_l|sentry_r|balance|dronse] [-v|-s]"
        exit 1
        ;;
esac

shift
if [ "$1" = "-v" ]; then
    verbose=1
fi

# ==============================================================
# 3. 编译阶段
# ==============================================================

echo -e "${yellow}<--- Start CMake (${ARMY_DEFINE}) --->${reset}"
cd build || exit 1
cmake -D CMAKE_CXX_FLAGS="${ARMY_DEFINE}" ..

echo -e "${yellow}\n<--- Start Make --->${reset}"
max_threads=$(nproc)
make -j "$max_threads"

echo -e "${yellow}\n<--- Total Lines --->${reset}"
echo -e "${blue}        $total${reset}"

cd ..

# ==============================================================
# 4. 模型文件安装
# ==============================================================

MODEL_DIR="$SYS_DIR/models"
sudo install -d -m755 "$MODEL_DIR"
sudo cp -f include/model/yolov7/rmyolov7-latest.xml "$MODEL_DIR"/ 2>/dev/null || true
sudo cp -f include/model/yolov7/rmyolov7-latest.bin "$MODEL_DIR"/ 2>/dev/null || true

# ==============================================================
# 5. 前端配置安装
# ==============================================================

FORWARD_CONFIG_DIR="$SYS_DIR/forward_config"
sudo install -d -m755 "$FORWARD_CONFIG_DIR"
sudo rm -rf "$FORWARD_CONFIG_DIR"/*

# 通用相机参数
sudo cp -f config/forward_config/camera_intrinsics.yaml "$FORWARD_CONFIG_DIR"/ 2>/dev/null || true
sudo cp -f config/forward_config/camera_extrinsics.yaml "$FORWARD_CONFIG_DIR"/ 2>/dev/null || true

# 兵种专属配置
echo -e "${yellow}拷贝 ${TARGET} 的配置文件...${reset}"
sudo cp -f config/${TARGET}/config.yaml "$FORWARD_CONFIG_DIR"/ 2>/dev/null || true
sudo cp -f config/${TARGET}/camera_intrinsics.yaml "$FORWARD_CONFIG_DIR"/ 2>/dev/null || true
sudo cp -f config/${TARGET}/camera_extrinsics.yaml "$FORWARD_CONFIG_DIR"/ 2>/dev/null || true

# ==============================================================
# 6. 安装与运行
# ==============================================================

if [ ! -f "GKD_Vision" ] && [ -f "build/visionlib" ]; then
    mv build/visionlib GKD_Vision
fi

if [ ! -f "GKD_Vision" ]; then
    echo -e "${red}❌ 未找到 GKD_Vision 可执行文件，编译可能失败${reset}"
    exit 1
fi

echo -e "${yellow}<--- 安装执行文件 --->${reset}"
sudo install -m755 -o root -g root GKD_Vision /usr/local/bin/GKD_Vision

# 杀掉旧进程
sudo pkill GKD_Vision 2>/dev/null || true

# 运行程序
echo -e "${yellow}<--- 启动 GKD_Vision (${TARGET}) --->${reset}"
if [ $verbose = 1 ]; then
    GKD_Vision -v
elif [ $imshow = 1 ]; then
    GKD_Vision -s
else
    GKD_Vision
fi

# ==============================================================
# 7. 创建守护脚本
# ==============================================================

GUARD_SCRIPT="$SYS_DIR/guard.sh"
if [ ! -f "$GUARD_SCRIPT" ]; then
    echo -e "${yellow}创建守护脚本: $GUARD_SCRIPT${reset}"
    sudo bash -c "cat > '$GUARD_SCRIPT' <<'EOF'
#!/bin/bash
# Guard script for GKD_Vision
EOF"
    sudo chmod 755 "$GUARD_SCRIPT"
fi

# 执行守护脚本（若存在逻辑）
if [ -f "$GUARD_SCRIPT" ]; then
    $GUARD_SCRIPT
fi

echo -e "${yellow}<----- ✅ OVER ----->${reset}"
