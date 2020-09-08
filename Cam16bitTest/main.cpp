#include "nkcOpenCV.h"
#include <iostream>

int main() {
	// 16ビットモードでカメラを開く
	int camID = 1;
	cv::VideoCapture cap(camID + cv::CAP_DSHOW);
	cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('Y', '1', '6', ' '));
	cap.set(cv::CAP_PROP_CONVERT_RGB, false);

	// カメラ画像のフォーマットチェック
	cv::Mat frame;
	cap >> frame;
	std::cout << cv::typeToString(frame.type()) << std::endl;

	// ウインドウの準備
	const cv::String WIN = "16bit camera";
	cv::namedWindow(WIN, cv::WINDOW_AUTOSIZE);

	// 周囲より10ドット内側を最大値・最小値算出のROIとする
	cv::Mat mask(frame.size(), CV_8UC1);
	mask = cv::Scalar(0);
	cv::rectangle(mask, cv::Rect(10, 10, 140, 100), cv::Scalar(255), cv::FILLED);

	while (true) {
		// 16ビット カメラ画像取得
		cv::Mat frame16;
		cap.read(frame16);

		// 最大値・最小値算出
		double minV, maxV;
		cv::minMaxIdx(frame16, &minV, &maxV, NULL, NULL, mask);
		//std::cout << minV << "," << maxV << std::endl;

		// 表示用に8ビットへ変換
		cv::Mat frame8;
		double alpha = 255.0 / (maxV - minV);
		double beta = -minV * alpha;
		frame16.convertTo(frame8, CV_8UC1, alpha, beta);

		// 4倍に拡大
		cv::Mat disp;
		cv::resize(frame8, disp, cv::Size(0, 0), 4.0, 4.0);

		// 表示
		cv::imshow(WIN, disp);
		if (cv::waitKey(1) == 27) break;
	}

	return 1;
}
