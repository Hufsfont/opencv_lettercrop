#include "opencv2/imgproc/imgproc.hpp"

#include "opencv2/imgcodecs.hpp"

#include "opencv2/highgui/highgui.hpp"

#include <iostream>

#include <opencv2/core/core.hpp>

#include<string>

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
/*�簢������ �߸� �̹��� computer ������Ʈ ���� ������ ����*/
//RGB�̹����� �簢�� �̸����������� �Ű������� ����
void ROI_save(Mat &Input, Rect rect, int cnt) {
	//�̹��� �簢������ �ʿ��� �κ� �ڸ���
	Mat roi = Input(rect);
	//���� �̸� ����
	string char_num = to_string(cnt);
	string name = "num" + char_num + ".jpg";
	//����
	imwrite(name, roi);
}
/*�̹��� ���ڿ� ���� �簢�� �׸���*/
//����ȭ �̹���(input)�� RGB�̹���(output)�� �Ű������� ����
Mat drawingRectangle(Mat &inputImg, Mat &output) {
	Mat Rectangle = output.clone(); //RGB�̹��� ����
	int cnt = 0; //�̹��� ���� �̸� ������ ���� ����

	//����ȭ �̹������� �ܰ��� ã��
	std::vector<vector<Point>> contours;
	std::vector<Vec4i> hierarchy;

	findContours(inputImg, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

	//�ܰ����� ���� �簢�� �׸���
	if (contours.size() > 0) {
		for (int idx = 0; idx < contours.size(); idx++) {
			Rect rect = boundingRect(contours[idx]);
			//�ʹ� �۰ų� �ʹ� ū �簢���� ����
			if (rect.width < 50 && rect.height >100 && rect.height * rect.width > 2000) { 
				rectangle(Rectangle, Point(rect.x, rect.y), Point(rect.x + rect.width, rect.y + rect.height), Scalar(0, 255, 0), 5);
				ROI_save(output, rect, cnt); //���� �Լ� �θ���
				cnt++;
			}
		}
	}
	return Rectangle; //�簢���� �׷��� �̹��� ��ȯ

}

int main()

{
	//�̹��� ������ �ҷ��� �׷��� �̹����� ��ȯ�Ѵ�.  
	Mat input_origin_image = imread("pengram02.jpg", IMREAD_COLOR);
	Mat input_gray_image;
	Mat result_binary_image;
	Mat kernel(3, 3, CV_8U, cv::Scalar(1));

	//�̹��� ������ ����
	resize(input_origin_image, input_origin_image, Size(500, 200), 0, 0);

	//���� �̹����� �׷��̽����� �̹����� ��ȯ
	cvtColor(input_origin_image, input_gray_image, COLOR_RGBA2GRAY);



	/*����ȭ*/
	Mat mask = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3), cv::Point(1, 1)); //delite���� kernal ũ��
	//�̹����� �ε巴�� ���� (�Է��̹���,����̹���,...)
	GaussianBlur(input_gray_image, input_gray_image, cv::Point(5, 5), 0);
	//�̹����� ����ȭ (�Է��̹���,����̹���,...)
	adaptiveThreshold(input_gray_image, result_binary_image, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 17, 3);
	//�̹��� ���� ���� (�Է��̹���,����̹���...)
	morphologyEx(result_binary_image, result_binary_image, cv::MORPH_CLOSE, kernel); //close
	//�� ����: deliate������ ����
	bitwise_not(result_binary_image, result_binary_image); 
	//��â ����, �̹����� �Ͼ�κ��� ��â��Ų�� (�Է��̹���, ����̹���,...,�ݺ�Ƚ��)
	dilate(result_binary_image, result_binary_image, mask, cv::Point(-1, -1), 3);
	
	//�簢�� �׷��� �̹��� �ޱ�
	Mat Rectangle = drawingRectangle(result_binary_image, input_origin_image);


	//���
	printWindow("�Է� �̹���", input_origin_image);
	printWindow("����ȭ �̹���", result_binary_image);
	printWindow("rectangle", Rectangle);

	//�ƹ�Ű�� ���� �� ���� ����Ѵ�.
	while (cv::waitKey(0) < 0);

}