#pragma once
#include "stdafx.h"
#include "FC2Wapper.h"
#include <FlyCapture2.h>
#include <iostream>
#include <opencv2\opencv.hpp>



void PrintCameraInfo(FlyCapture2::CameraInfo *pCamInfo)
{
	std::cout << std::endl;
	std::cout << "*** CAMERA INFORMATION ***" << std::endl;
	std::cout << "Serial number - " << pCamInfo->serialNumber << std::endl;
	std::cout << "Camera model - " << pCamInfo->modelName << std::endl;
	std::cout << "Camera vendor - " << pCamInfo->vendorName << std::endl;
	std::cout << "Sensor - " << pCamInfo->sensorInfo << std::endl;
	std::cout << "Resolution - " << pCamInfo->sensorResolution << std::endl;
	std::cout << "Firmware version - " << pCamInfo->firmwareVersion << std::endl;
	std::cout << "Firmware build time - " << pCamInfo->firmwareBuildTime << std::endl
		<< std::endl;
}


FC2Cam::FC2Cam()
{
	//カメラの数を取得する
	error = busMgr.GetNumOfCameras(&numCameras);
	if (error != FlyCapture2::PGRERROR_OK) {
		error.PrintErrorTrace();
	}

	std::cout << "Number of cameras detected: " << numCameras << std::endl;
	pCameras = new Camera[numCameras];
	raw_image = new Image[numCameras];
	converted_image = new Image[numCameras];
	//カメラの接続
	for (unsigned int i = 0; i < numCameras; i++) {
		error = busMgr.GetCameraFromIndex(i, &guid);
		if (error != PGRERROR_OK) {
			error.PrintErrorTrace();
			return;
		}
		error = pCameras[i].Connect(&guid);
		if (error != PGRERROR_OK) {
			error.PrintErrorTrace();
			return;
		}
	}
	//カメラが接続されてない時はデストラクタを呼ぶ
	if (numCameras == 0)
		this->~FC2Cam();

}

FC2Cam::~FC2Cam()
{
	delete[] pCameras;
	delete[] raw_image;
	delete[] converted_image;
	pCameras = nullptr;
	raw_image = nullptr;
	converted_image = nullptr;

}

//bool FC2Cam::connect()
//{
//	for (unsigned int i = 0; i < numCameras; i++) {
//		error = busMgr.GetCameraFromIndex(i, &guid);
//		if (error != PGRERROR_OK) {
//			error.PrintErrorTrace();
//			return false;
//		}
//		error = pCameras[i].Connect(&guid);
//		if (error != PGRERROR_OK) {
//			error.PrintErrorTrace();
//			return false;
//		}
//	}
//	return true;
//}

void FC2Cam::disconnect()
{
	for (unsigned int i = 0; i < numCameras; i++) {
		pCameras[i].StopCapture();
		pCameras[i].Disconnect();
	}
}

bool FC2Cam::get_camera_info()
{
	CameraInfo cam_info;
	for (unsigned int i = 0; i < numCameras; i++) {
		error = pCameras[i].GetCameraInfo(&cam_info);
		if (error != PGRERROR_OK) {
			error.PrintErrorTrace();
			return false;
		}
		PrintCameraInfo(&cam_info);
	}
	return true;
}

void FC2Cam::setFC2_Property(FC2_Property & prop)
{
	set_VMode_and_FRate(prop.Video_Mode, prop.Frame_Rate);
	if (prop.Exporsure != 0)
		autoExposuer(false, prop.Exporsure);
	if (prop.White_Balance != cv::Vec2i(0, 0))
		autoWhiteBalance(false, prop.White_Balance[0],prop.White_Balance[1]);
	if (prop.Satulation != 0)
		autoSatulation(false, prop.Satulation);
	if (prop.Shutter != 0)
		autoShutter(false, prop.Shutter);
	if (prop.Gain != 0)
		autoGain(false, prop.Gain);
	if (prop.fps != 0)
		autoFrameRate(false, prop.fps);
	set_triggerMode(prop.triggerMode);
	return;
		
}

void FC2Cam::autoExposuer(bool flag, float absValue)
{
	Property prop;
	prop.type = AUTO_EXPOSURE;
	prop.onOff = true;
	prop.autoManualMode = flag;
	prop.absControl = true;
	prop.absValue = absValue;
	for (unsigned int i = 0; i < numCameras; i++) {
		error = pCameras[i].SetProperty(&prop);
		if (error != PGRERROR_OK) {
			error.PrintErrorTrace();
		}
	}
}

void FC2Cam::autoWhiteBalance(bool flag, int red, int blue)
{
	Property prop;
	prop.type = WHITE_BALANCE;
	prop.onOff = true;
	prop.autoManualMode = flag;
	prop.valueA = red;
	prop.valueB = blue;
	for (unsigned int i = 0; i < numCameras; i++) {
		error = pCameras[i].SetProperty(&prop);
		if (error != PGRERROR_OK) {
			error.PrintErrorTrace();
		}
	}
}

void FC2Cam::autoSatulation(bool flag, float percent)
{
	Property prop;
	prop.type = SATURATION;
	prop.onOff = true;
	prop.autoManualMode = flag;
	prop.absControl = true;
	prop.absValue = percent;
	for (unsigned int i = 0; i < numCameras; i++) {
		error = pCameras[i].SetProperty(&prop);
		if (error != PGRERROR_OK) {
			error.PrintErrorTrace();
		}
	}
}

void FC2Cam::autoShutter(bool flag, float ms)
{
	Property prop;
	prop.type = SHUTTER;
	prop.autoManualMode = flag;
	prop.absControl = true;
	prop.absValue = ms;
	for (unsigned int i = 0; i < numCameras; i++) {
		error = pCameras[i].SetProperty(&prop);
		if (error != PGRERROR_OK) {
			error.PrintErrorTrace();
		}
	}
}

void FC2Cam::autoGain(bool flag, float gain)
{
	Property prop;
	prop.type = GAIN;
	prop.autoManualMode = flag;
	prop.absControl = true;
	prop.absValue = gain;
	for (unsigned int i = 0; i < numCameras; i++) {
		error = pCameras[i].SetProperty(&prop);
		if (error != PGRERROR_OK) {
			error.PrintErrorTrace();
		}
	}
}

void FC2Cam::autoFrameRate(bool flag, float fps)
{
	Property prop;
	prop.type = FRAME_RATE;
	prop.autoManualMode = flag;
	prop.absControl = true;
	prop.absValue = fps;
	for (unsigned int i = 0; i < numCameras; i++) {
		error = pCameras[i].SetProperty(&prop);
		if (error != PGRERROR_OK) {
			error.PrintErrorTrace();
		}
	}
}

void FC2Cam::set_triggerMode(bool state)
{
	TriggerMode trigMode;
	trigMode.onOff = state;
	for (unsigned int i = 0; i < numCameras; i++) {
		error = pCameras[i].SetTriggerMode(&trigMode);
		if (error != PGRERROR_OK) {
			error.PrintErrorTrace();
		}
	}
}

bool FC2Cam::start_capture(FC2_Property&prop)
{	
	setFC2_Property(prop);
	for (unsigned int i = 0; i < numCameras; i++) {
		error = pCameras[i].StartCapture();
		if (error != PGRERROR_OK) {
			error.PrintErrorTrace();
			return false;
		}
	}
	return true;
}

//std::vector<cv::Mat> FC2Cam::get_frame_mat()
//{
//	std::vector<cv::Mat> outMat;
//	for (unsigned int i = 0; i < numCameras; i++) {
//		error = pCameras[i].RetrieveBuffer(&raw_image[i]);
//		if (error != PGRERROR_OK) {
//			error.PrintErrorTrace();
//		}
//		raw_image[i].Convert(FlyCapture2::PIXEL_FORMAT_BGR, &converted_image[i]);
//		cv::Mat dstframe((int)raw_image[i].GetRows(), (int)raw_image[i].GetCols(), CV_8UC3, converted_image[i].GetData());
//		outMat.push_back(dstframe);
//	}
//	return outMat;
//}

//std::vector<cv::UMat> FC2Cam::get_frame_umat()
//{
//	std::vector<cv::UMat> outUMat;
//	for (unsigned int i = 0; i < numCameras; i++) {
//		error = pCameras[i].RetrieveBuffer(&raw_image[i]);
//		if (error != PGRERROR_OK) {
//			error.PrintErrorTrace();
//		}
//		error = raw_image[i].Convert(FlyCapture2::PIXEL_FORMAT_BGR, &converted_image[i]);
//		cv::Mat dstframe((int)raw_image[i].GetRows(), (int)raw_image[i].GetCols(), CV_8UC3, converted_image[i].GetData());
//		outUMat.push_back(dstframe.getUMat(cv::ACCESS_READ));
//		
//	}
//	return outUMat;
//
//}

//bool FC2Cam::get_VMode_and_FRate()
//{
//
//	for (unsigned int i = 0; i < numCameras; i++) {
//		error = pCameras[i].GetVideoModeAndFrameRate(&video_mode, &frame_rate);
//		if (error != FlyCapture2::PGRERROR_OK) {
//			error.PrintErrorTrace();
//			return false;
//		}
//	}
//	
//	return true;
//}



bool FC2Cam::set_VMode_and_FRate(FlyCapture2::VideoMode v_mode, FlyCapture2::FrameRate f_rate)
{

	for (unsigned int i = 0; i < numCameras; i++) {
		error = pCameras[i].SetVideoModeAndFrameRate(v_mode, f_rate);
		if (error != FlyCapture2::PGRERROR_OK) {
			error.PrintErrorTrace();
			return false;
		}
	}

	return true;
}

void FC2Cam::operator >> (std::vector<cv::Mat>& outMat)
{
	for (unsigned int i = 0; i < numCameras; i++) {
		error = pCameras[i].RetrieveBuffer(&raw_image[i]);
		if (error != PGRERROR_OK) {
			error.PrintErrorTrace();
		}
		raw_image[i].Convert(FlyCapture2::PIXEL_FORMAT_BGR, &converted_image[i]);
		cv::Mat dstframe((int)raw_image[i].GetRows(), (int)raw_image[i].GetCols(), CV_8UC3, converted_image[i].GetData());
		outMat.push_back(dstframe);
	}
}

void FC2Cam::operator >> (std::vector<cv::UMat>& outUMat) {

	for (unsigned int i = 0; i < numCameras; i++) {
		error = pCameras[i].RetrieveBuffer(&raw_image[i]);
		if (error != PGRERROR_OK) {
			error.PrintErrorTrace();
		}
		error = raw_image[i].Convert(FlyCapture2::PIXEL_FORMAT_BGR, &converted_image[i]);
		cv::Mat dstframe((int)raw_image[i].GetRows(), (int)raw_image[i].GetCols(), CV_8UC3, converted_image[i].GetData());
		outUMat.push_back(dstframe.getUMat(cv::ACCESS_READ));

	}
}


