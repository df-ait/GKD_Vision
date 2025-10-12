#ifndef RM2026_GARAGE_WRAPPER_CAR_H_
#define RM2026_GARAGE_WRAPPER_CAR_H_

#include "garage/interface.h"

class WrapperCar : public ObjInterface {

public:
    WrapperCar(rm::ArmorID id);
    ~WrapperCar() {};
    void push(const rm::Target& target, TimePoint t) override;
    void update() override;
    bool getTarget(Eigen::Vector4d& pose, const double fly_delay, const double rotate_delay, const double shoot_delay) override;
    rm::ArmorSize getArmorSize() override;
    void getState(std::vector<std::string>& lines) override;
    void setState(int state) override {}

public:
    rm::TrackQueueV4 track_queue_;
    rm::AntitopV3* antitop_2_;//针对平衡步兵，2装甲板
    rm::AntitopV3* antitop_4_;//针对普通车辆，4装甲板

    bool   flag_big_armor_ = false;         // 是否为大装甲板
    int    armor_size_count_ = 0;           // 装甲板尺寸计数
    int    curr_armor_num_ = 0;             // 当前一次更新内观测的装甲板数量
    
    //核心模式切换标志
    bool flag_antitop_ = false; //是否启用反陀螺模式 
    bool flag_center_ = false;  //仅在反陀螺模式下有效，true表示瞄准机器人中心，false为瞄准装甲板

    //控制反陀螺模式和普通跟踪模式之间切换的角速度阈值
    double antitop_to_track_ = 0.6;
    double track_to_antitop_ = 1.0;

    //控制反陀螺模式内瞄准装甲板or中心的角速度阈值
    double armor_to_center_ = 0.7;
    double center_to_armor_ = 0.6;
};

#endif