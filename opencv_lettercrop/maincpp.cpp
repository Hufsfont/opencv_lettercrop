#include "opencv2/imgproc/imgproc.hpp"

#include "opencv2/imgcodecs.hpp"

#include "opencv2/highgui/highgui.hpp"

#include <iostream>

#include <opencv2/core/core.hpp>

#include<string>

using namespace cv;
using namespace std;

void printWindow(string nameTag, Mat inputImg) {

	namedWindow(nameTag, WINDOW_AUTOSIZE);
	resize(inputImg, inputImg, Size(500, 200));
	imshow(nameTag, inputImg);

}
void ROI_save(Mat &Input, Rect rect, int cnt) {
	Mat roi = Input(rect);
	string char_num = to_string(cnt);
	string name = "num" + char_num + ".jpg";
	imwrite(name, roi);
}

Mat drawingRectangle(Mat &inputImg, Mat &output) {
	Mat Rectangle = output.clone();
	int cnt = 0;

	std::vector<vector<Point>> contours;
	std::vector<Vec4i> hierarchy;

	findContours(inputImg, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

	if (contours.size() > 0) {
		for (int idx = 0; idx < contours.size(); idx++) {
			Rect rect = boundingRect(contours[idx]);
			
			if (rect.width < 50 && rect.height >100 && rect.height * rect.width > 2000) { 
				rectangle(Rectangle, Point(rect.x, rect.y), Point(rect.x + rect.width, rect.y + rect.height), Scalar(0, 255, 0), 5);
				ROI_save(output, rect, cnt);
				cnt++;
			}
		}
	}
	return Rectangle;

}

int main()

{
	//이미지 파일을 불러와 그레이 이미지로 변환한다.  
	Mat input_origin_image = imread("pengram02.jpg", IMREAD_COLOR);
	Mat input_gray_image;
	Mat result_binary_image;
	Mat kernel(3, 3, CV_8U, cv::Scalar(1));

	resize(input_origin_image, input_origin_image, Size(500, 200), 0, 0);

	cvtColor(input_origin_image, input_gray_image, COLOR_RGBA2GRAY);



	//이진화
	Mat mask = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3), cv::Point(1, 1));
	GaussianBlur(input_gray_image, input_gray_image, cv::Point(5, 5), 0);
	adaptiveThreshold(input_gray_image, result_binary_image, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 17, 3);
	printWindow("adapt 이미지", result_binary_image);
	morphologyEx(result_binary_image, result_binary_image, cv::MORPH_CLOSE, kernel); //close
	printWindow("morph 이미지", result_binary_image);
	bitwise_not(result_binary_image, result_binary_image); //색 반전
	
	dilate(result_binary_image, result_binary_image, mask, cv::Point(-1, -1), 3);
	

	Mat Rectangle = drawingRectangle(result_binary_image, input_origin_image);


	//출력
	printWindow("입력 이미지", input_origin_image);
	printWindow("이진화 이미지", result_binary_image);
	printWindow("rectangle", Rectangle);

	//아무키나 누를 때 까지 대기한다.
	while (cv::waitKey(0) < 0);

}