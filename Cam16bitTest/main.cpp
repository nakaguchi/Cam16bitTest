#include "nkcOpenCV.h"
#include <iostream>

int main() {
	// 16�r�b�g���[�h�ŃJ�������J��
	int camID = 1;
	cv::VideoCapture cap(camID + cv::CAP_DSHOW);
	cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('Y', '1', '6', ' '));
	cap.set(cv::CAP_PROP_CONVERT_RGB, false);

	// �J�����摜�̃t�H�[�}�b�g�`�F�b�N
	cv::Mat frame;
	cap >> frame;
	std::cout << cv::typeToString(frame.type()) << std::endl;

	// �E�C���h�E�̏���
	const cv::String WIN = "16bit camera";
	cv::namedWindow(WIN, cv::WINDOW_AUTOSIZE);

	// ���͂��10�h�b�g�������ő�l�E�ŏ��l�Z�o��ROI�Ƃ���
	cv::Mat mask(frame.size(), CV_8UC1);
	mask = cv::Scalar(0);
	cv::rectangle(mask, cv::Rect(10, 10, 140, 100), cv::Scalar(255), cv::FILLED);

	while (true) {
		// 16�r�b�g �J�����摜�擾
		cv::Mat frame16;
		cap.read(frame16);

		// �ő�l�E�ŏ��l�Z�o
		double minV, maxV;
		cv::minMaxIdx(frame16, &minV, &maxV, NULL, NULL, mask);
		//std::cout << minV << "," << maxV << std::endl;

		// �\���p��8�r�b�g�֕ϊ�
		cv::Mat frame8;
		double alpha = 255.0 / (maxV - minV);
		double beta = -minV * alpha;
		frame16.convertTo(frame8, CV_8UC1, alpha, beta);

		// 4�{�Ɋg��
		cv::Mat disp;
		cv::resize(frame8, disp, cv::Size(0, 0), 4.0, 4.0);

		// �\��
		cv::imshow(WIN, disp);
		if (cv::waitKey(1) == 27) break;
	}

	return 1;
}
