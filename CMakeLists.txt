cmake_minimum_required(VERSION 3.15)
project(GKD_Vision LANGUAGES CXX)

# ------------------------------------------------------------------------------
# 0. 基本配置
# ------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)
add_compile_options(-O3 -w)

message(STATUS "===============================================")
message(STATUS "       Building project: ${PROJECT_NAME}      ")
message(STATUS "===============================================")

# ------------------------------------------------------------------------------
# 1. 查找并配置 OpenCV
# ------------------------------------------------------------------------------
find_package(OpenCV REQUIRED)
if (NOT OpenCV_FOUND)
    message(FATAL_ERROR "未找到 OpenCV，请先安装或配置 OpenCV！")
endif()
message(STATUS "Using OpenCV version ${OpenCV_VERSION}")

# ------------------------------------------------------------------------------
# 2. 配置 OpenVINO
# ------------------------------------------------------------------------------
# 如有需要，可根据安装版本修改路径:
set(OpenVINODeveloperPackage_DIR "/usr/lib/cmake/openvino2024.6.0")
find_package(OpenVINO REQUIRED)
if (NOT DEFINED openvino_VERSION)
    message(WARNING "OpenVINO found, but version not detected.")
else()
    message(STATUS "Using OpenVINO version ${openvino_VERSION}")
endif()

# ------------------------------------------------------------------------------
# 3. 查找 Eigen3
# ------------------------------------------------------------------------------
find_package(Eigen3 REQUIRED)
if (EIGEN3_FOUND)
    if (DEFINED EIGEN3_INCLUDE_DIRS)
        message(STATUS "Found Eigen3 include dirs: ${EIGEN3_INCLUDE_DIRS}")
        include_directories(${EIGEN3_INCLUDE_DIRS})
    elseif(DEFINED EIGEN3_INCLUDE_DIR)
        message(STATUS "Found Eigen3 include dir: ${EIGEN3_INCLUDE_DIR}")
        include_directories(${EIGEN3_INCLUDE_DIR})
    else()
        message(WARNING "Eigen3 include dir variable not set, fallback to /usr/include/eigen3")
        include_directories(/usr/include/eigen3)
    endif()
else()
    message(WARNING "未找到 Eigen3，尝试手动包含默认路径 /usr/include/eigen3")
    include_directories(/usr/include/eigen3)
endif()

# ------------------------------------------------------------------------------
# 4. 配置海康相机 SDK
# ------------------------------------------------------------------------------
# 假设 64 位库都放在 lib/64 目录下
set(CAMERA_LIB_DIR "${CMAKE_SOURCE_DIR}/lib/64")
link_directories(${CAMERA_LIB_DIR})

# ------------------------------------------------------------------------------
# 5. 包含头文件目录
# ------------------------------------------------------------------------------
include_directories(
    ${CMAKE_SOURCE_DIR}/include              # 项目公共头文件
    ${CMAKE_SOURCE_DIR}/lib/include          # 第三方库头文件
    ${CMAKE_SOURCE_DIR}/lib/nlohmann         # JSON 库头文件
    ${CMAKE_SOURCE_DIR}/lib                  # 如有直接放在 lib 下的头文件
    ${OpenCV_INCLUDE_DIRS}                   # OpenCV 头文件
    # ${OpenVINO_INCLUDE_DIRS}               # 通常 find_package(OpenVINO) 已经处理
)

# ------------------------------------------------------------------------------
# 6. 源文件收集
# ------------------------------------------------------------------------------
file(GLOB_RECURSE SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

set(MAIN_FILE ${CMAKE_SOURCE_DIR}/main.cpp)
# ------------------------------------------------------------------------------
# 7. 生成可执行文件
# ------------------------------------------------------------------------------
add_executable(visionlib ${MAIN_FILE} ${SOURCE_FILES})

# ------------------------------------------------------------------------------
# 8. 链接依赖库
# ------------------------------------------------------------------------------
target_link_libraries(visionlib
    ${OpenCV_LIBS}
    openvino::runtime

    # 海康相机驱动相关库（需根据 SDK 实际情况调整）
    MvCameraControl
    MVGigEVisionSDK
    FormatConversion
    GCBase_gcc421_v3_0
    GenApi_gcc421_v3_0
    log4cpp_gcc421_v3_0
    Log_gcc421_v3_0
    MathParser_gcc421_v3_0
    MediaProcess
    MVRender
    MvUsb3vTL
    NodeMapData_gcc421_v3_0
    XmlParser_gcc421_v3_0

    X11
    pthread
)

# ------------------------------------------------------------------------------
# 9. 编译器选项（可选）
# ------------------------------------------------------------------------------
if (CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(visionlib PRIVATE -Wall)
endif()

# ------------------------------------------------------------------------------
# 10. OpenVINO 模板插件（可选）
# ------------------------------------------------------------------------------
option(ENABLE_TEMPLATE_REGISTRATION "Enable TEMPLATE plugin registration" OFF)
if (ENABLE_TEMPLATE_REGISTRATION)
    target_compile_definitions(visionlib PRIVATE ENABLE_TEMPLATE_REGISTRATION)
endif()

# ------------------------------------------------------------------------------
# 11. 安装配置文件（可选）
# ------------------------------------------------------------------------------
install(FILES config/infantry/config.yaml DESTINATION /etc/gkd_vision/infantry)

message(STATUS "===============================================")
message(STATUS "  ✅ CMake configuration complete!")
message(STATUS "  Project: ${PROJECT_NAME}")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "===============================================")
