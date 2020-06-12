#include "opencv2/imgproc/imgproc.hpp"

#include "opencv2/imgcodecs.hpp"

#include "opencv2/highgui/highgui.hpp"

#include <iostream>

#include <opencv2/core/core.hpp>

#include <string>


using namespace cv;
using namespace std;


/*�̹��� computer ��� �Լ�*/
void printWindow(string nameTag, Mat inputImg) {
	//������ â �����
	namedWindow(nameTag, WINDOW_AUTOSIZE);
	//���� ������ ����
	resize(inputImg, inputImg, Size(500, 200));
	//�̹��� ���
	imshow(nameTag, inputImg);

}

int main()

{
	//�̹��� ������ �ҷ��� �׷��� �̹����� ��ȯ�Ѵ�.  
	Mat input_origin_image = imread("pengram03.jpg", IMREAD_COLOR);
	Mat input_gray_image;
	Mat result_binary_image;
	Mat kernel(3, 3, CV_8U, cv::Scalar(1));

	//�̹��� ������ ����
	resize(input_origin_image, input_origin_image, Size(10000, 500), INTER_AREA);

	//���� �̹����� �׷��̽����� �̹����� ��ȯ
	cvtColor(input_origin_image, input_gray_image, COLOR_RGBA2GRAY);



	/*����ȭ*/
	Mat mask = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7), cv::Point(1, 1)); //delite���� kernal ũ��

	//�̹����� ����ȭ (�Է��̹���,����̹���,...)
	adaptiveThreshold(input_gray_image, result_binary_image, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 27, 7);
	printWindow("�̹���", result_binary_image);
	//�̹��� ���� ���� (�Է��̹���,����̹���...)
	morphologyEx(result_binary_image, result_binary_image, cv::MORPH_CLOSE, kernel); //close
	//�� ����: deliate������ ����
	bitwise_not(result_binary_image, result_binary_image);
	//��â ����, �̹����� �Ͼ�κ��� ��â��Ų�� (�Է��̹���, ����̹���,...,�ݺ�Ƚ��)
	dilate(result_binary_image, result_binary_image, mask, cv::Point(-1, -1), 5);


	//����ȭ �̹������� �ܰ��� ã��
	std::vector<vector<Point>> contours;
	std::vector<Vec4i> hierarchy;

	findContours(result_binary_image, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE, Point(0, 0));

	Rect roiRect[10];//�˸��� �簢�� �迭
	if (contours.size() > 0) {
		int hier;
		for (int idx = 0; idx < contours.size(); idx++) {
			Rect rect = boundingRect(contours[idx]);
			if (rect.width > 2000) {
				hier = idx;
				//rectangle(input_origin_image, Point(rect.x, rect.y), Point(rect.x + rect.width, rect.y + rect.height), Scalar(0, 255, 0), 7); //�����
			}
		} //�Ϸ�
		for (int i = 0, j = 0; i < contours.size(); i++) {
			//printf("hier: %d, i: %d H: %d\n", hier, i, hierarchy[i][3]);
			if (hierarchy[i][3] == hier) {
				roiRect[j] = boundingRect(contours[i]);
				//rectangle(input_origin_image, Point(roiRect[j].x, roiRect[j].y), Point(roiRect[j].x + roiRect[j].width, roiRect[j].y + roiRect[j].height), Scalar(0, 0, 255), 7); //�����
				j++;
			}
		}
	}
	
	//������� ����
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10 - i - 1; j++) {
			if (roiRect[j].x > roiRect[j + 1].x) {
				Rect bff = roiRect[j];
				roiRect[j] = roiRect[j + 1];
				roiRect[j + 1] = bff;
			}
		}
	}
	//����
	for (int i = 0; i < 10; i++) {
		Mat roi = input_origin_image(roiRect[i]); //CROP
		resize(roi, roi, Size(1000, 1000), INTER_LINEAR);
		string char_num = to_string(i);
		string name = "num" + char_num + ".jpg";
		imwrite(name, roi);
	}

	//���
	printWindow("����ȭ �̹���", result_binary_image);
	printWindow("rectangle", input_origin_image);

	//�ƹ�Ű�� ���� �� ���� ����Ѵ�.
	while (cv::waitKey(0) < 0);

}