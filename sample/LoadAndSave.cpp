#include "stdafx.h"
#include "LoadAndSave.h"


using namespace std;

//カメラパラメータをxmlファイルから読み込む
//	intrinsic:
//  distortion:歪みねえな(ある)
//	image_size:画像のサイズ
void loadCameraParam(char* argv, cv::Mat& intrinsic, cv::Mat& distortion, cv::Size& image_size) {
	cv::FileStorage fs(argv, cv::FileStorage::READ);
	fs["camera_matrix"] >> intrinsic;
	fs["distortion_coefficients"] >> distortion;
	fs["image_width"] >> image_size.width;
	fs["image_height"] >> image_size.height;
	if (!fs.isOpened() || intrinsic.empty() || distortion.empty()) {
		cout << "Error: Couldn't load intrinsic parameters from xml/yaml file." << endl;
		return;
	}
	fs.release();

}

//画像の読み込み
//argc:コマンドライン引数の数
//argv:コマンドライン引数のポインタのポインタ
//inputMat:読み込んだ画像を入れるベクタ
//
int loadImage(int argc, char ** argv, std::vector<cv::Mat>& inputMat)
{
	for (int i = 4; i < argc; i++) {
		inputMat.push_back(cv::imread(argv[i]));
	}
	if (inputMat.empty())
		return -1;
}

void get_date(std::string& str) {
	time_t rawtime; time(&rawtime);
	tm tmlocal;
	char ascstr[32];
	localtime_s(&tmlocal, &rawtime);
	asctime_s(ascstr, sizeof(ascstr), &tmlocal);
	std::string tmpstr(ascstr);
	str = tmpstr;
}
void get_date_file_format(std::string& str) {
	time_t rawtime; time(&rawtime);
	tm tmlocal;
	char tmpchar[32];
	localtime_s(&tmlocal, &rawtime);
	//asctime_s(str, sizeof(str), &tmlocal);
	strftime(tmpchar, sizeof(tmpchar), "%y%m%d%H%M%S", &tmlocal);
	str = tmpchar;
}

std::string get_date_file_format()
{
	time_t rawtime; time(&rawtime);
	tm tmlocal;
	char tmpchar[32];
	localtime_s(&tmlocal, &rawtime);
	//asctime_s(str, sizeof(str), &tmlocal);
	strftime(tmpchar, sizeof(tmpchar), "%y%m%d%H%M%S", &tmlocal);
	return tmpchar;
}

void save_raw_position_and_rotation(vector<CameraPosition>& CamVector)
{
	cv::FileStorage fs("./result/raw_position_data.yml", cv::FileStorage::APPEND);
	string ascstr;
	get_date(ascstr);
	fs << "Position estimate time" << ascstr;
	for (int i = 0; i < CamVector.size(); i++) {
		fs << "image" << i;
		fs << "result" << "[";
		fs << "Position" << CamVector[i].position;
		fs << "Rotation" << CamVector[i].rotation;
		fs << "]";
	}
	fs.release();
}

void save_result_to_yml(ManageCamClass & Clustor)
{

	cv::FileStorage fs("./result/result.yml", cv::FileStorage::APPEND);
	std::string ascstr;
	get_date(ascstr);
	fs << "Camera estimation time" << ascstr;
	for (int i=0; i < Clustor.CameraClustor.size(); i++) {
		fs << "Camera" << i;
		fs << "result"<<"[";
		fs << "Position" << Clustor.CameraClustor[i].position;
		fs << "Rotation" << Clustor.CameraClustor[i].rotation;
		fs << "]";
	}
	fs.release();
}
