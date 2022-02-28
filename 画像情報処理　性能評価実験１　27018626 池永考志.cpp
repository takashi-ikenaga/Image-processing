//性能評価実験1
//学籍番号：27018626
//氏名：池永考志
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(void)
{
	VideoCapture camera;//カメラ画像を取得するためのクラス
	camera.open(0);

	//スライダーバーで選択する最大最小設定
	//色相
	int hueMin = 0;
	int hueMax = 181;
	//彩度
	int satMin = 0;
	int satMax = 255;
	//明度
	int valMin = 0;
	int valMax = 255;

	//オープニング、クロージングで設定する、回数という変数の初期化
	int opening = 0;
	int closing = 0;

	//いろんな設定のためのスライダーバー
	namedWindow("SliderBar");
	resizeWindow("SliderBar", 700, 400);
	createTrackbar("Hue Min", "SliderBar", &hueMin, 181);
	createTrackbar("Hue Max", "SliderBar", &hueMax, 181);
	createTrackbar("Sat Min", "SliderBar", &satMin, 256);
	createTrackbar("Sat Max", "SliderBar", &satMax, 256);
	createTrackbar("Val Min", "SliderBar", &valMin, 256);
	createTrackbar("Val Max", "SliderBar", &valMax, 256);
	createTrackbar("Opening", "SliderBar", &opening, 10);//飛び飛びの画素の補正
	createTrackbar("Closing", "SliderBar", &closing, 10);//穴あき補正

	//各画像処理用の配列設定
	vector<int> linear_array;
	vector< vector<Point> > contours;
	double area;
	vector<vector<Point>> polygons;
	vector<Point> polygon;

	while (1)
	{
		Mat rgbImage, hsvImage, maskImage,reverseImage; //表示する各画像の名前の定義

		camera >> rgbImage; //カメラ画像を処理前のところに代入している

		cvtColor(rgbImage, hsvImage, CV_BGR2HSV); //rgbからhsvに変換
		inRange(hsvImage, Scalar(hueMin, satMin, valMin), Scalar(hueMax, satMax, valMax), maskImage); //特定色画素の抽出

		//オープニング処理
		erode(maskImage, maskImage, Mat(), Point(-1, -1), opening); //収縮処理を行うもの
		dilate(maskImage, maskImage, Mat(), Point(-1, -1), opening); //膨張処理を行うもの

		//クロージング処理
		dilate(maskImage, maskImage, Mat(), Point(-1, -1), closing); //膨張処理を行うもの
		erode(maskImage, maskImage, Mat(), Point(-1, -1), closing); //収縮処理を行うもの

	
		findContours(maskImage, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
		// mask_image（二値画像が格納済とする）中で，最も外側の輪郭線を抽出
	
		drawContours(rgbImage, contours, -1, Scalar(255, 0,0), 1, 8);
		// 上で得られたすべての輪郭線を，input_image（カラー画像が格納済とする）中に青色で描画


		Mat resultImage(rgbImage.cols, rgbImage.rows, CV_8UC3); //resultImageの定義

		for (int i = 0; i < contours.size(); i++) {    // contour（輪郭線データが格納済とする）に含まれる輪郭線数の繰り返し

			area = contourArea(Mat(contours[i])); // i番目の輪郭線の面積を算出
			resultImage.setTo(Scalar(0, 0, 0)); //違いがわかるように画面を黒く設定

			if (area >= 15000) //輪郭線で囲われた面積が指定した値以上に存在するなら、という条件
			{
				approxPolyDP(Mat(contours[i]), polygon, 10, true); // i番目の輪郭線を誤差10の閉じた多角形で近似
				polygons.push_back(polygon); //データを追加している
				rgbImage.copyTo(resultImage, maskImage);//maskImageの白い位置のrgbImageをresultImageに貼り付け
			}
		}

		bitwise_not(resultImage, reverseImage);//反転した画素を持つ画像イメージを作成
		//画像の名前付け、画像を表示
		imshow("Color rgbImage", rgbImage);
		imshow("Mask Image", maskImage);
		imshow("Result Image", resultImage);
		imshow("Reverse Image", reverseImage);

		if (waitKey(1) == 'q') break; //終了コマンド
	}

	return 0;
}