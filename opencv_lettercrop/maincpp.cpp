#include "opencv2/imgproc/imgproc.hpp"

#include "opencv2/imgcodecs.hpp"

#include "opencv2/highgui/highgui.hpp"

#include <iostream>

#include <opencv2/core/core.hpp>

#include <string>


using namespace cv;
using namespace std;


/*이미지 computer 출력 함수*/
void printWindow(string nameTag, Mat inputImg) {
	//윈도우 창 만들기
	namedWindow(nameTag, WINDOW_AUTOSIZE);
	//사진 사이즈 통일
	resize(inputImg, inputImg, Size(500, 200));
	//이미지 출력
	imshow(nameTag, inputImg);

}

int main()

{
	//이미지 파일을 불러와 그레이 이미지로 변환한다.  
	Mat input_origin_image = imread("pengram01.jpg", IMREAD_COLOR);
	Mat input_gray_image;
	Mat result_binary_image;
	Mat kernel(3, 3, CV_8U, cv::Scalar(1));

	//이미지 사이즈 조절
	resize(input_origin_image, input_origin_image, Size(10000, 500), 0, 0);

	//원본 이미지를 그레이스케일 이미지로 변환
	cvtColor(input_origin_image, input_gray_image, COLOR_RGBA2GRAY);



	/*이진화*/
	Mat mask = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5), cv::Point(1, 1)); //delite연산 kernal 크기
	//이미지를 부드럽게 만듦 (입력이미지,출력이미지,...)
	GaussianBlur(input_gray_image, input_gray_image, cv::Point(5, 5), 0);
	//이미지를 이진화 (입력이미지,출력이미지,...)
	adaptiveThreshold(input_gray_image, result_binary_image, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 29, 7);
	//이미지 잡음 제거 (입력이미지,출력이미지...)
	morphologyEx(result_binary_image, result_binary_image, cv::MORPH_CLOSE, kernel); //close
	//색 반전: deliate연산을 위해
	bitwise_not(result_binary_image, result_binary_image); 
	//팽창 연산, 이미지의 하얀부분을 팽창시킨다 (입력이미지, 출력이미지,...,반복횟수)
	dilate(result_binary_image, result_binary_image, mask, cv::Point(-1, -1), 5);
	
	//이진화 이미지에서 외곽선 찾기
	std::vector<vector<Point>> contours;
	std::vector<Vec4i> hierarchy;

	findContours(result_binary_image, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

	/////////////////////////////////////cnt변수를 i로 바꿨음(의미 NO)
	//외곽선에 맞춰 사각형 찾기 
	if (contours.size() > 0) {
		for (int idx = 0, i=0; idx < contours.size(); idx++) {
			Rect rect = boundingRect(contours[idx]);
			//알맞은 사각형 조건 (수정)
			if (rect.width < 2000 && rect.height > 290 && rect.height * rect.width > 200000) {
				//printf("x: %d, y: %d, width: %d, height: %d\n", rect.x, rect.y, rect.width, rect.height); //디버깅
				//rectangle 함수는 사각형이 잘 그려지는 지 눈으로 보려고 쓰던거, 삭제해도 상관없음
				//rectangle(input_origin_image, Point(rect.x, rect.y), Point(rect.x + rect.width, rect.y + rect.height), Scalar(0, 255, 0), 7);
				
				
				Mat roi = input_origin_image(rect); //CROP
				//저장 이름 생성
				string char_num = to_string(i);
				string name = "num" + char_num + ".jpg";
				//저장
				imwrite(name, roi);
				i++;
			}
		}

	}


	//출력
	printWindow("이진화 이미지", result_binary_image);
	printWindow("rectangle", input_origin_image);

	//아무키나 누를 때 까지 대기한다.
	while (cv::waitKey(0) < 0);

}