#include <iostream>
#include <Eigen/Dense>
#include <utils/timer.h>
#include "utils/print.h"
#include "kalman/interface/error_calculator.h"
#include <memory>
#include <cmath>
//#include "uniterm/uniterm.h"

using namespace rm;
using namespace std;

//安全计算角度差
double ErrorCalculator::getSafeAngle(const double angle1,const double angle2){
    double angle = angle1 - angle2;
    while(angle > M_PI) angle -= 2 * M_PI;
    while(angle < -M_PI) angle += 2 * M_PI;
    return angle;
}

//根据误差结果计算误差分析
error_states ErrorCalculator::getErrorStats(){
    error_states_.total_count = error_result_.size();
    double all_pos = 0,all_angle;
    for (auto i:error_result_)
    {
        all_pos += i.pos_error;
        all_angle += i.angle_error;
        if(i.is_error_valid){
            error_states_.valid_count++;
        }
    }
    error_states_.avg_pos_error = all_pos / error_result_.size();
    error_states_.avg_angle_error = all_angle / error_result_.size();
    error_states_.max_pos_error = -1;
    error_states_.min_pos_error = 10000000.0;
    double std_all_pos = 0,std_all_angle = 0;
    for (auto i:error_result_)
    {
        if(i.pos_error > error_states_.max_pos_error){
            error_states_.max_pos_error = i.pos_error;
        }
        if(i.pos_error < error_states_.min_pos_error){
            error_states_.min_pos_error = i.pos_error;
        }
        std_all_pos += pow(error_states_.avg_pos_error - i.pos_error,2);
        std_all_angle += pow(error_states_.avg_angle_error - i.angle_error,2);
    }
    error_states_.std_pos_error = sqrt(std_all_pos / error_result_.size() );
    error_states_.std_angle_error = sqrt(std_all_angle / error_result_.size() );
    return error_states_;
}

double ErrorCalculator::getDoubleOfS(const TimePoint& start, const TimePoint& end) {
    return getDuration_s(start, end).count();
}

double ErrorCalculator::getDistance(const Eigen::Matrix<double, 3, 1>& this_pose, const Eigen::Matrix<double, 3, 1>& last_pose){
    double dx = this_pose(0) - last_pose(0);
    double dy = this_pose(1) - last_pose(1);
    double dz = this_pose(2) - last_pose(2);
    double d = std::sqrt(dx * dx + dy * dy + dz * dz);
    return d;
}

double ErrorCalculator::getDistance(const Eigen::Matrix<double, 4, 1>& this_pose, const Eigen::Matrix<double, 4, 1>& last_pose) {
    double dx = this_pose(0) - last_pose(0);
    double dy = this_pose(1) - last_pose(1);
    double dz = this_pose(2) - last_pose(2);
    double d = std::sqrt(dx * dx + dy * dy + dz * dz);
    return d;
}

//打印误差信息
void ErrorCalculator::Print_error(error_result error_result_){
    std::cout<<"pos_error: "<<error_result_.pos_error<<std::endl;
    std::cout<<"angle_error: "<<error_result_.angle_error<<std::endl;
    //std::cout<<"combine_error: "<<error_result_.combin_error<<std::endl;
    std::cout<<"delay_time: "<<error_result_.delay_time <<std::endl;
    std::cout<<"last_predict_pose[x,y,z,angle]: ["<<error_result_.last_predic_pose(0)<<","
                                                <<error_result_.last_predic_pose(1)<<","
                                                <<error_result_.last_predic_pose(2)<<","
                                                <<error_result_.last_predic_pose(3) <<"]"<<std::endl;
    std::cout<<"now_pose[x,y,z,angle]: ["<<error_result_.now_pose(0)<<","
                                                <<error_result_.now_pose(1)<<","
                                                <<error_result_.now_pose(2)<<","
                                                <<error_result_.now_pose(3) <<"]"<<std::endl;
    return;
}

error_result ErrorCalculator::calculerror3D(const Eigen::Matrix<double, 3, 1> prediction , const Eigen::Matrix<double , 3, 1 > view,
TimePoint predict_ , TimePoint view_){
    error_result error_result_s;
    error_result_s.now_pose = view;
    error_result_s.last_predic_pose = prediction;
    error_result_s.pos_error = getDistance(prediction,view);
    error_result_s.view_time = view_;
    error_result_s.predict_time = predict_;
    error_result_s.delay_time = getDoubleOfS(error_result_s.view_time,error_result_s.predict_time);
    error_result_.push_back(error_result_s);
    return error_result_s;
}

error_result ErrorCalculator::calculerror4D(const Eigen::Matrix<double, 4, 1> prediction , const Eigen::Matrix<double , 4, 1 > view,
TimePoint predict_ , TimePoint view_){
    error_result error_result_s;
    error_result_s.now_pose = view;
    error_result_s.last_predic_pose = prediction;
    error_result_s.pos_error = getDistance(prediction,view);
    error_result_s.angle_error = getSafeAngle(prediction(3),view(3));
    error_result_s.view_time = view_;
    error_result_s.predict_time = predict_;
    error_result_s.delay_time = getDoubleOfS(error_result_s.view_time,error_result_s.predict_time);
    error_result_.push_back(error_result_s);
    return error_result_s;
}