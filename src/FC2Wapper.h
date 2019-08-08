#pragma once

#include<FlyCapture2.h>
#include<opencv2\opencv.hpp>
#include<vector>
#include<memory>
using namespace FlyCapture2;

struct FC2_Property {
	FrameRate Frame_Rate;
	VideoMode Video_Mode;
	float Exporsure;
	cv::Vec2i White_Balance;
	float Satulation;
	float Shutter;
	float Gain;
	float fps;
	bool triggerMode;

	FC2_Property() :
		Frame_Rate(FRAMERATE_30), Video_Mode(VIDEOMODE_640x480Y8), Exporsure(0), White_Balance(0),
		Satulation(0), Shutter(0), Gain(0), fps(0), triggerMode(false) {};

	FC2_Property(VideoMode in_Video_Mode, FrameRate in_Frame_rate) :
		Video_Mode(in_Video_Mode), Frame_Rate(in_Frame_rate), Exporsure(0), White_Balance(0),
		Satulation(0), Shutter(0), Gain(0), fps(0), triggerMode(false) {};

	FC2_Property(
		VideoMode in_Video_Mode,
		FrameRate in_Frame_Rate,
		float in_Exporsure,
		cv::Vec2i in_White_Balance,
		float in_Satulation,
		float in_Shutter,
		float in_Gain,
		float in_fps,
		bool in_triggerMode
	) :Video_Mode(in_Video_Mode), Frame_Rate(in_Frame_Rate), Exporsure(in_Exporsure), White_Balance(in_White_Balance),
		Satulation(in_Satulation), Shutter(in_Shutter), Gain(in_Gain), fps(in_fps), triggerMode(in_triggerMode) {};
};


//***FC2Cam Class***
//
class FC2Cam {
private:
	PGRGuid guid;
public:
	Camera *pCameras;
	BusManager busMgr;
	unsigned int numCameras;
	Error error;
	Image *raw_image;
	Image *converted_image;
	
	 

	//コンストラクタ
	FC2Cam();
	//デストラクタ
	~FC2Cam();
	//カメラに接続
	bool connect();
	//カメラを切断
	void disconnect();
	//カメラの情報を取得する
	bool get_camera_info();

	//FC2_Propertyをセット
	void setFC2_Property(FC2_Property& prop);

	//露出オート
	void autoExposuer(bool, float);
	//オートホワイトバランス
	void autoWhiteBalance(bool, int, int);
	//オートsatulation
	void autoSatulation(bool ,float);
	//オートシャッタースピード
	void autoShutter(bool, float);
	//オートゲイン
	void autoGain(bool, float);
	//オートフレームレート
	void autoFrameRate(bool, float);

	//トリガーモードを設定
	void set_triggerMode(bool state);

	//キャプチャ開始
	bool start_capture(FC2_Property &);

	//VideoModeAndFrameRateをカメラに設定
	bool set_VMode_and_FRate(VideoMode v_mode, FrameRate f_rate);

	//1フレーム読み込んでMatに渡す
	void operator >> (std::vector<cv::Mat>&);

	//1フレーム読み込んでUMatに渡す
	void operator >> (std::vector<cv::UMat>&);
};

void PrintCameraInfo(CameraInfo *pCamInfo);