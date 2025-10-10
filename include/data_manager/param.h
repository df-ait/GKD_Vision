#ifndef RM2026_DATA_MANAGER_PARAM_H_
#define RM2026_DATA_MANAGER_PARAM_H_

#include <opencv2/opencv.hpp>
#include <memory>
#include <string>
#include <fstream>
#include <exception>
#include "json.hpp"

class Param {
public:
    explicit Param(const std::string& json_path) { 
        if (!load(json_path)) {
            // Initialize with an empty JSON object if config file fails to load
            params_ = nlohmann::json::object();
        }
    }
    static std::shared_ptr<Param> get_instance() {
        static std::shared_ptr<Param> instance(new Param());
        return instance;
    }
    
    bool load(const std::string&);
    void dump(const std::string&);

    nlohmann::json& operator[](const std::string&);

    static void from_json(const nlohmann::json& j, cv::Mat& p);
    static void to_json(nlohmann::json& j, const cv::Mat& p);

private:
    Param() { 
        if (!load(default_path_)) {
            // Initialize with an empty JSON object if config file fails to load
            params_ = nlohmann::json::object();
        }
    }
    Param(const Param&) = delete;
    Param& operator=(const Param&) = delete;

private:
    nlohmann::json params_;
    std::string default_path_ = "/etc/visionlib/Config.json";


};


#endif