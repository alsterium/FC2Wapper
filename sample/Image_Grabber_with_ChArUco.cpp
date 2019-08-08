// Image_Grabber_with_ChArUco.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//
#pragma once

#include "stdafx.h"
#include <vector>
#include <opencv2\opencv.hpp>
#include <opencv2\aruco\charuco.hpp>
#include "FC2Wapper.h"
#include "LoadAndSave.h"

using namespace std;

int main()
{
	FC2Cam cam;
	FC2_Property Prop(VIDEOMODE_1280x960Y8, FRAMERATE_7_5);
	cam.start_capture(Prop);


	cv::Mat camMatrix, distCoeffs;
	cv::Size image_size;


	//カメラパラメータをファイルから読み込む
	loadCameraParam("intrinsics1280x960.xml", camMatrix, distCoeffs, image_size);

	cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::PREDEFINED_DICTIONARY_NAME(0));
	cv::Ptr<cv::aruco::CharucoBoard> charucoboard = cv::aruco::CharucoBoard::create(7, 5, 0.40, 0.20, dictionary);


	while (1) {
		vector<cv::Mat> raw_image;
		//画像を取得
		cam >> raw_image;
		int image_n = raw_image.size();

		//各変数の宣言
		vector<vector<int>> markerIds(image_n), charucoIds(image_n);
		vector<int> interpolatedCorners(image_n);
		vector<bool> validPose(image_n);
		vector<vector<vector<cv::Point2f>>> markerCorners(image_n), rejectedMarkers(image_n);
		vector<vector<cv::Point2f>> charucoCorners(image_n);
		vector<cv::Mat>rvec(image_n), tvec(image_n);


		//raw_imageの各画像について推定を行う
		for (int i = 0; i < image_n; i++) {
			//ChArUcoボードを特定する
			//
			cv::aruco::detectMarkers(
				raw_image[i],
				dictionary,
				markerCorners[i],
				markerIds[i],
				cv::aruco::DetectorParameters::create(),
				rejectedMarkers[i],
				camMatrix,
				distCoeffs
			);

			//ChArUcoボードを補完する
			if (markerIds[i].size() > 0)
				interpolatedCorners[i] = cv::aruco::interpolateCornersCharuco(
					markerCorners[i],
					markerIds[i],
					raw_image[i],
					charucoboard,
					charucoCorners[i],
					charucoIds[i],
					camMatrix,
					distCoeffs
				);
			//ChArUcoボードの姿勢の推定
			validPose[i] = false;
			validPose[i] = cv::aruco::estimatePoseCharucoBoard(
				charucoCorners[i],
				charucoIds[i],
				charucoboard,
				camMatrix,
				distCoeffs,
				rvec[i],
				tvec[i]
			);

		}

		//結果を表示する
		//
		vector<cv::Mat> result_image;
		result_image = raw_image;
		for (int i = 0; i < image_n; i++) {
			//結果を表示
			//if (markerIds[i].size() > 0)
			//	cv::aruco::drawDetectedMarkers(result_image[i], markerCorners[i]);

			//if (interpolatedCorners[i] > 0) {
			//	cv::Scalar color;
			//	color = cv::Scalar(255, 0, 0);
			//	cv::aruco::drawDetectedCornersCharuco(result_image[i], charucoCorners[i], charucoIds[i], color);
			//}

			//if (validPose[i])
			//	cv::aruco::drawAxis(result_image[i], camMatrix, distCoeffs, rvec[i], tvec[i], 0.5f);

			if (validPose[i])
				cout << "\n camera " << i << "is validPose" << endl;
		}


		for (int i = 0; i < image_n; i++) {
			cv::resize(result_image[i], result_image[i], cv::Size(), 0.5f, 0.5f);
			cv::String WindowName = "result " + to_string(i);
			cv::imshow(WindowName, result_image[i]);
		}

		if (cv::waitKey(33) == 'c') {
			for (int i = 0; i < image_n; i++) {
				if (validPose[i]) {
					cv::String FileName = "./result/image_" + to_string(i) + get_date_file_format() + ".jpg";
					cv::imwrite(FileName, raw_image[i]);
				}
			}

		}

		if (cv::waitKey(33) == 27)break;

	}


	return 0;
}

