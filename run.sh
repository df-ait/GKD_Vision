#!/bin/bash

blue="\033[1;34m"
yellow="\033[1;33m"
reset="\033[0m"

include_count=$(find include  -type f \( -name "*.cpp" -o -name "*.h" \) -exec cat {} \; | wc -l)
src_count=$(find src  -type f \( -name "*.cpp" -o -name "*.h" -o -name "*.txt" \) -exec cat {} \; | wc -l)
total=$((include_count + src_count))

if [ ! -d "/etc/ visionlib" ]; then 
    sudo mkdir -p /etc/ visionlib
    sudo chmod -R 777 /etc/ visionlib
fi

if [ ! -d "config" ]; then 
    ln -s /etc/ visionlib ./config
fi

if [ ! -d "build" ]; then 
    mkdir build
fi

imshow=0
verbose=0

# 注意：这里 *不再* 把 v 写进 getopts 的选项列表中
while getopts ":rcg:ls" opt; do
    case $opt in
        r)
            echo -e "${yellow}<--- delete 'build' --->\n${reset}"
            sudo rm -rf build
            mkdir build
            ;;
        c)
            sudo cp -r data/uniconfig/* /etc/ visionlib/
            sudo chmod -R 777 /etc/ visionlib
            exit 0
            ;;
        g)
            git_message=$OPTARG
            echo -e "${yellow}\n<--- Git $git_message --->${reset}"
            git pull
            git add -A
            git commit -m "$git_message"
            git push
            exit 0
            ;;
        l)
            cd ../ visionlib
            sudo ./run.sh
            cd ../GKD_Vision
            exit 0
            ;;
        s)
            imshow=1
            ;;
        \?)
            echo -e "${red}\n--- Unavailable param: -$OPTARG ---\n${reset}"
            ;;
        :)
            echo -e "${red}\n--- param -$OPTARG need a value ---\n${reset}"
            ;;
    esac
done

# 将所有已处理过的选项移除，剩下的即是非选项参数
shift $((OPTIND - 1))

echo -e "${yellow}<--- Start CMake --->${reset}"
cd build
cmake ..

echo -e "${yellow}\n<--- Start Make --->${reset}"
max_threads=$(nproc)
make -j "$max_threads"

echo -e "${yellow}\n<--- Total Lines --->${reset}"
echo -e "${blue}        $total${reset}"

# ========== 新增：模型文件安装 ==========
MODEL_DIR="/etc/ visionlib/models"
if [ ! -d "${MODEL_DIR}" ]; then
    sudo mkdir -p ${MODEL_DIR}
    sudo chmod -R 777 /etc/ visionlib
fi

# 复制本地模型文件到系统目录
sudo cp -f include/model/yolov7/rmyolov7-latest.xml ${MODEL_DIR}/ 2>/dev/null || true
sudo cp -f include/model/yolov7/rmyolov7-latest.bin ${MODEL_DIR}/ 2>/dev/null || true



# ========== 新增：前端配置文件安装 ==========
FORWARD_CONFIG_DIR="/etc/ visionlib/forward_config"
if [ ! -d "${FORWARD_CONFIG_DIR}" ]; then
    sudo mkdir -p "${FORWARD_CONFIG_DIR}"
    sudo chmod -R 777 /etc/ visionlib
fi

sudo rm -rf "${FORWARD_CONFIG_DIR}"/*

# 复制通用相机标定文件
sudo cp -f config/forward_config/camera_intrinsics.yaml "${FORWARD_CONFIG_DIR}/" 2>/dev/null || true
sudo cp -f config/forward_config/camera_extrinsics.yaml "${FORWARD_CONFIG_DIR}/" 2>/dev/null || true

# 第一个非选项参数是兵种
TARGET="$1"
case "$TARGET" in
    hero|infantry|sentry_l|sentry_r)
        echo "Copy ${TARGET} config to ${FORWARD_CONFIG_DIR} ..."
        # 复制兵种特定配置（覆盖同名文件）
        sudo cp -f config/${TARGET}/config.yaml "${FORWARD_CONFIG_DIR}/"
        # 同时也复制兵种特定的相机参数（如果存在）
        sudo cp -f config/${TARGET}/camera_intrinsics.yaml "${FORWARD_CONFIG_DIR}/" 2>/dev/null || true
        sudo cp -f config/${TARGET}/camera_extrinsics.yaml "${FORWARD_CONFIG_DIR}/" 2>/dev/null || true
        ;;
    *)
        echo "无效或未指定兵种参数，跳过拷贝前端配置文件"
        exit 1
        ;;
esac

# 再 shift 掉兵种参数，然后看下一个是否是 -v
shift
if [ "$1" = "-v" ]; then
    verbose=1
fi

# 检查可执行文件是否存在
if [ ! -f "GKD_Vision" ]; then
    echo -e "${red}Error: GKD_Vision executable not found. Build may have failed.${reset}"
    exit 1
fi

sudo rm /usr/local/bin/GKD_Vision 2>/dev/null || true
sudo cp GKD_Vision /usr/local/bin/
sudo pkill GKD_Vision 2>/dev/null || true
sudo chmod 777 /dev/tty* 2>/dev/null || true

# 根据是否选择 -v / -s 决定调用
if [ $verbose = 1 ]; then
    GKD_Vision -v
elif [ $imshow = 1 ]; then
    GKD_Vision -s
else
    GKD_Vision
fi

# 创建 guard.sh 如果不存在
GUARD_SCRIPT="/etc/ visionlib/guard.sh"
if [ ! -f "$GUARD_SCRIPT" ]; then
    sudo mkdir -p /etc/ visionlib
    echo '#!/bin/bash' | sudo tee "$GUARD_SCRIPT" > /dev/null
    echo '# Guard script for GKD_Vision' | sudo tee -a "$GUARD_SCRIPT" > /dev/null
    sudo chmod +x "$GUARD_SCRIPT"
fi

# 运行 guard 脚本
if [ -f "$GUARD_SCRIPT" ]; then
    $GUARD_SCRIPT
fi

echo -e "${yellow}<----- OVER ----->${reset}"