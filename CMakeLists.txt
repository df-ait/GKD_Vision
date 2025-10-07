cmake_minimum_required(VERSION 3.15)
project(GKD_Vision)

# 设置编译指令
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE DEBUG)
add_compile_options(-g3)

# 目标兵种保持要选的即可
add_definitions(-DGKD_INFANTRY -DVISON_DEBUG=0)

# ============= 必须保留的原有配置 =============
# OpenRM核心库配置（必须保留！）
find_package(OpenRM REQUIRED)
include_directories(${OpenRM_INCLUDE_DIRS})

# ============= 依赖配置 =============
# OpenCV
find_package(OpenCV 4.5.4 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Eigen
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

# Ceres
find_package(Ceres REQUIRED)
include_directories(${CERES_INCLUDE_DIRS})

# OpenVINO配置
set(OpenVINODeveloperPackage_DIR "/usr/lib/cmake/openvino2024.6.0")
find_package(OpenVINO REQUIRED)
if (NOT DEFINED openvino_VERSION)
    message(WARNING "OpenVINO was found, but version could not be determined.")
else()
    message(STATUS "Using OpenVINO version ${openvino_VERSION}")
endif()

# 项目头文件
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/libs/json/single_include/nlohmann
    ${CMAKE_SOURCE_DIR}/include/video/HIK/include
)

# ============= 收集所有源文件 =============
file(GLOB_RECURSE ALL_SOURCE_FILES
    "src/*.cpp"
)

# 合并所有源文件
set(SOURCE_FILES
    main.cpp
    ${ALL_SOURCE_FILES}
)

# 可执行文件
add_executable(GKD_Vision 
    ${SOURCE_FILES}
)

# ============= 链接库顺序调整 =============
target_link_libraries(GKD_Vision
    # 原有依赖
    ${OpenRM_LIBRARIES}  # 必须保留！
    # OpenCV
    ${OpenCV_LIBS}
    # OpenVINO
    openvino::runtime
    # 系统库
    X11
    pthread
    dl
)

# 安装配置保持原有
install(TARGETS GKD_Vision
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)