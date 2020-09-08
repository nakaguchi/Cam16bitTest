#include "nkcOpenCV.h"
#include <Windows.h>

namespace nkc {
namespace ocv {

// 待たないwaitKey()
int noWaitKey() {
	MSG msg;
	while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
		if (::GetMessage(&msg, NULL, 0, 0) == 0) {
			// WM_QUIT
			::PostQuitMessage(msg.wParam);
			return 0;
		}

		if (msg.message == WM_CHAR) return (int)msg.wParam;

		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
	return -1;
}

// 複数画像を縦横に連結表示
void imShowMulti(
	cv::String winname, std::vector<cv::Mat>& imgs, // 全ての表示画像（8bit 3ch or 8bit 1chのみ)
	unsigned int cols, // 横の連結数
	unsigned int rows, // 縦の連結数
	cv::Size imgsize, // 表示する画像サイズ
	unsigned int border) {
	if (imgs.size() < 1 || cols < 1 || rows < 1) return;

	unsigned int w = imgsize.width + border, h = imgsize.height + border;
	cv::Mat board(h * rows + border, w * cols + border, CV_8UC3, CV_RGB(128, 128, 128));
	for (unsigned int r = 0, i = 0; r < rows; r++) {
		for (unsigned int c = 0; c < cols; c++, i++) {
			cv::Rect roi_rect = cv::Rect(c * w + border, r * h + border, imgsize.width, imgsize.height);
			cv::Mat roi(board, roi_rect);
			if (i < imgs.size()) {
				if (imgs[i].type() == CV_8UC3) {
					resize(imgs[i], roi, imgsize);
				} else if (imgs[i].type() == CV_8UC1) {
					cv::Mat c3;
					cvtColor(imgs[i], c3, cv::COLOR_GRAY2BGR);
					resize(c3, roi, imgsize);
				} else {
					putText(roi, "Color mode not matched", cv::Point(20, 20), cv::FONT_HERSHEY_COMPLEX, 0.5, CV_RGB(0, 0, 0));
				}
			} else {
				putText(roi, "No image", cv::Point(20, 20), cv::FONT_HERSHEY_COMPLEX, 0.5, CV_RGB(0, 0, 0));
			}
		}
	}
	imshow(winname, board);
}

// 縦横比を保ったリサイズ
cv::Mat KeepAspectResize(cv::Mat& img, int width, int height) {
	cv::Size outputSize;
	if (width > 0) {
		if (height > 0) {
			// 縦横指定（両サイズに収まるようにする）
			outputSize = cv::Size(height * img.cols / img.rows, width * img.rows / img.cols);
			if (outputSize.width > width) outputSize.width = width;
			else outputSize.height = height;
		} else {
			// 横幅指定
			outputSize = cv::Size(width, width * img.rows / img.cols);
		}
	} else {
		if (height > 0) {
			// 縦幅指定
			outputSize = cv::Size(height * img.cols / img.rows, height);
		} else {
			// 縦横どちらも指定無い場合は入力のコピーを返す
			return img.clone();
		}
	}
	cv::Mat resized;
	cv::resize(img, resized, outputSize);
	return resized;
}

// ウインドウハンドルを取得
HANDLE getWindowHandle(cv::String winname) {
	return ::FindWindowA(NULL, winname.c_str());
}


}; // namespace ocv
}; // namespace nkc
