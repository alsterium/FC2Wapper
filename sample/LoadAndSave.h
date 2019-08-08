#pragma once
#include "stdafx.h"
#include <iostream>
#include <opencv2\opencv.hpp>
#include <vector>
#include "ManageCamClass.h"

void loadCameraParam(char* argv, cv::Mat& intrinsic, cv::Mat& distortion, cv::Size& image_size);

int loadImage(int argc, char** argv, std::vector<cv::Mat>& inputMat);

void god_save_the_queen();

void save_result_to_yml(ManageCamClass &Clustor);

void get_date(std::string& str);

void get_date_file_format(std::string& str);

std::string get_date_file_format();

void save_raw_position_and_rotation(std::vector<CameraPosition>& CamVector);