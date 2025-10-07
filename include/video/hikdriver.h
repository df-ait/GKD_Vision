#ifndef OPENRM_VIDEO_HIKDRIVER_H
#define OPENRM_VIDEO_HIKDRIVER_H

#include <iostream>
#include <X11/Xlib.h> 
#include <assert.h>  
#include "math.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h> 
#include "MvCameraControl.h" // 海康相机SDK头文件，在HIK下的include目录

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <pthread.h>
#include "data_manager/parameter_loader.h"

extern cv::Mat HIKimage;
extern std::mutex HIKframemtx;

int HIKcamtask();

#endif