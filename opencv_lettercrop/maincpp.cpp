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
	Mat input_origin_image = imread("pengram01.jpg", IMREAD_COLOR);
	Mat input_gray_image;
	Mat result_binary_image;
	Mat kernel(3, 3, CV_8U, cv::Scalar(1));

	//�̹��� ������ ����
	resize(input_origin_image, input_origin_image, Size(10000, 500), 0, 0);

	//���� �̹����� �׷��̽����� �̹����� ��ȯ
	cvtColor(input_origin_image, input_gray_image, COLOR_RGBA2GRAY);



	/*����ȭ*/
	Mat mask = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5), cv::Point(1, 1)); //delite���� kernal ũ��
	//�̹����� �ε巴�� ���� (�Է��̹���,����̹���,...)
	//GaussianBlur(input_gray_image, input_gray_image, cv::Point(5, 5), 0);
	//�̹����� ����ȭ (�Է��̹���,����̹���,...)
	adaptiveThreshold(input_gray_image, result_binary_image, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 29, 3);
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

	findContours(result_binary_image, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

	/////////////////////////////////////cnt������ i�� �ٲ���(�ǹ� NO)
	//�ܰ����� ���� �簢�� ã�� 
	if (contours.size() > 0) {
		for (int idx = 0, i=0; idx < contours.size(); idx++) {
			Rect rect = boundingRect(contours[idx]);
			printf("x: %d, y: %d, width: %d, height: %d\n", rect.x, rect.y, rect.width, rect.height);
			//�˸��� �簢�� ���� (����)
			if (rect.width < 2000 && rect.height > 200 && rect.height * rect.width > 160000) {
				printf("***x: %d, y: %d, width: %d, height: %d\n", rect.x, rect.y, rect.width, rect.height); //�����
				//rectangle �Լ��� �簢���� �� �׷����� �� ������ ������ ������, �����ص� �������
				rectangle(input_origin_image, Point(rect.x, rect.y), Point(rect.x + rect.width, rect.y + rect.height), Scalar(0, 255, 0), 7);
				
				
				Mat roi = input_origin_image(rect); //CROP
				//���� �̸� ����
				string char_num = to_string(i);
				string name = "num" + char_num + ".jpg";
				//����
				imwrite(name, roi);
				i++;
			}
		}

	}


	//���
	printWindow("����ȭ �̹���", result_binary_image);
	printWindow("rectangle", input_origin_image);

	//�ƹ�Ű�� ���� �� ���� ����Ѵ�.
	while (cv::waitKey(0) < 0);

}