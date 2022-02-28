//���\�]������1
//�w�Дԍ��F27018626
//�����F�r�i�l�u
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(void)
{
	VideoCapture camera;//�J�����摜���擾���邽�߂̃N���X
	camera.open(0);

	//�X���C�_�[�o�[�őI������ő�ŏ��ݒ�
	//�F��
	int hueMin = 0;
	int hueMax = 181;
	//�ʓx
	int satMin = 0;
	int satMax = 255;
	//���x
	int valMin = 0;
	int valMax = 255;

	//�I�[�v�j���O�A�N���[�W���O�Őݒ肷��A�񐔂Ƃ����ϐ��̏�����
	int opening = 0;
	int closing = 0;

	//�����Ȑݒ�̂��߂̃X���C�_�[�o�[
	namedWindow("SliderBar");
	resizeWindow("SliderBar", 700, 400);
	createTrackbar("Hue Min", "SliderBar", &hueMin, 181);
	createTrackbar("Hue Max", "SliderBar", &hueMax, 181);
	createTrackbar("Sat Min", "SliderBar", &satMin, 256);
	createTrackbar("Sat Max", "SliderBar", &satMax, 256);
	createTrackbar("Val Min", "SliderBar", &valMin, 256);
	createTrackbar("Val Max", "SliderBar", &valMax, 256);
	createTrackbar("Opening", "SliderBar", &opening, 10);//��є�т̉�f�̕␳
	createTrackbar("Closing", "SliderBar", &closing, 10);//�������␳

	//�e�摜�����p�̔z��ݒ�
	vector<int> linear_array;
	vector< vector<Point> > contours;
	double area;
	vector<vector<Point>> polygons;
	vector<Point> polygon;

	while (1)
	{
		Mat rgbImage, hsvImage, maskImage,reverseImage; //�\������e�摜�̖��O�̒�`

		camera >> rgbImage; //�J�����摜�������O�̂Ƃ���ɑ�����Ă���

		cvtColor(rgbImage, hsvImage, CV_BGR2HSV); //rgb����hsv�ɕϊ�
		inRange(hsvImage, Scalar(hueMin, satMin, valMin), Scalar(hueMax, satMax, valMax), maskImage); //����F��f�̒��o

		//�I�[�v�j���O����
		erode(maskImage, maskImage, Mat(), Point(-1, -1), opening); //���k�������s������
		dilate(maskImage, maskImage, Mat(), Point(-1, -1), opening); //�c���������s������

		//�N���[�W���O����
		dilate(maskImage, maskImage, Mat(), Point(-1, -1), closing); //�c���������s������
		erode(maskImage, maskImage, Mat(), Point(-1, -1), closing); //���k�������s������

	
		findContours(maskImage, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
		// mask_image�i��l�摜���i�[�ςƂ���j���ŁC�ł��O���̗֊s���𒊏o
	
		drawContours(rgbImage, contours, -1, Scalar(255, 0,0), 1, 8);
		// ��œ���ꂽ���ׂĂ̗֊s�����Cinput_image�i�J���[�摜���i�[�ςƂ���j���ɐF�ŕ`��


		Mat resultImage(rgbImage.cols, rgbImage.rows, CV_8UC3); //resultImage�̒�`

		for (int i = 0; i < contours.size(); i++) {    // contour�i�֊s���f�[�^���i�[�ςƂ���j�Ɋ܂܂��֊s�����̌J��Ԃ�

			area = contourArea(Mat(contours[i])); // i�Ԗڂ̗֊s���̖ʐς��Z�o
			resultImage.setTo(Scalar(0, 0, 0)); //�Ⴂ���킩��悤�ɉ�ʂ������ݒ�

			if (area >= 15000) //�֊s���ň͂�ꂽ�ʐς��w�肵���l�ȏ�ɑ��݂���Ȃ�A�Ƃ�������
			{
				approxPolyDP(Mat(contours[i]), polygon, 10, true); // i�Ԗڂ̗֊s�����덷10�̕������p�`�ŋߎ�
				polygons.push_back(polygon); //�f�[�^��ǉ����Ă���
				rgbImage.copyTo(resultImage, maskImage);//maskImage�̔����ʒu��rgbImage��resultImage�ɓ\��t��
			}
		}

		bitwise_not(resultImage, reverseImage);//���]������f�����摜�C���[�W���쐬
		//�摜�̖��O�t���A�摜��\��
		imshow("Color rgbImage", rgbImage);
		imshow("Mask Image", maskImage);
		imshow("Result Image", resultImage);
		imshow("Reverse Image", reverseImage);

		if (waitKey(1) == 'q') break; //�I���R�}���h
	}

	return 0;
}