#pragma once

#include <iostream>
#include <Eigen/Dense>
#include <utils/timer.h>
#include "utils/print.h"
#include <memory>
#include <vector>
#include <utils/timer.h>
#include <kalman/filter/ekf.h>
#include <structure/slidestd.hpp>
//误差综合
struct error_states
{
    double avg_pos_error = 0;   //平均位置误差
    double avg_angle_error = 0; //平均角度误差
    double std_pos_error = 0;   //位置误差标准茶
    double std_angle_error = 0; //角度误差标准茶
    double max_pos_error = 0;   //最大位置误差
    double min_pos_error = -1.0;   //最小位置误差
    int valid_count = 0;    //有效计算次数
    int total_count = 0;    //总的计算次数
};

//误差计算结果
struct error_result
{
    Eigen::Matrix<double,4,1> now_pose;//delay_time后的实际位姿
    Eigen::Matrix<double,4,1>last_predic_pose; //依据之前观测位姿预测的delay_time后的预测位姿
    double pos_error = -1.0;   //位置误差
    double angle_error = -1.0; //角度误差
    double delay_time = 0.0; //预测延迟的时间
    bool is_error_valid = false;   //误差是否有效
    TimePoint predict_time; //预测时间
    TimePoint view_time;    //观测时间
};

class ErrorCalculator{
public:
    ErrorCalculator(){};

    //误差计算结果(改为vector存储了)
    //error_result error_result_;
    std::vector<error_result> error_result_;
    
    //误差综合
    error_states error_states_;
    
    error_result calculerror3D(const Eigen::Matrix<double, 3, 1> prediction , const Eigen::Matrix<double , 3, 1 > view,TimePoint predict_ , TimePoint view_);//传入预测值和这个时刻的观测值，计算三个维度误差x,y,z
    error_result calculerror4D(const Eigen::Matrix<double, 4, 1> prediction , const Eigen::Matrix<double , 4, 1 > view, TimePoint predict_ , TimePoint view_);
    error_states getErrorStats();//算error_states

    //打印误差信息
    void Print_error(error_result error_result_);
private:
    double getSafeAngle(const double angle1,const double angle2);
    double getDistance(const Eigen::Matrix<double, 3, 1>& this_pose, const Eigen::Matrix<double, 3, 1>& last_pose);
    double getDistance(const Eigen::Matrix<double, 4, 1>& this_pose, const Eigen::Matrix<double, 4, 1>& last_pose);
    double getDoubleOfS(const TimePoint& start, const TimePoint& end);
};