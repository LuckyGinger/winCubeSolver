#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
//#include <opencv2/videoio.hpp>
//#include <opencv2/objdetect.hpp>

#include <iostream>

using namespace std;
using namespace cv;

void drawLines(Mat image)
{
	// draw Cube lines on live feed

	int centerY = image.rows / 2;
	int centerX = image.cols / 2;


	for (int y = 0; y < image.rows; y++) // y = rows
	{

		for (int x = 0; x < image.cols; x++) // x = cols
		{

			Vec3b bgrPixel = image.at<Vec3b>(y, x);

			// Pink lines
			if (x == centerX - 51 || x == centerX - 50 || x == centerX - 49 ||
				x == centerX + 51 || x == centerX + 50 || x == centerX + 49 ||
				y == centerY - 51 || y == centerY - 50 || y == centerY - 49 ||
				y == centerY + 51 || y == centerY + 50 || y == centerY + 49)
			{
				bgrPixel[0] = 241;
				bgrPixel[1] = 66;
				bgrPixel[2] = 244;

				image.at<Vec3b>(y, x) = bgrPixel;
			}

			// Black lines
			if ((x == centerX - 151 || x == centerX - 150 || x == centerX - 149 ||
				x == centerX + 151 || x == centerX + 150 || x == centerX + 149 ||
				y == centerY - 151 || y == centerY - 150 || y == centerY - 149 ||
				y == centerY + 151 || y == centerY + 150 || y == centerY + 149) &&
				x >= centerX - 151 && x <= centerX + 151 &&
				y >= centerY - 151 && y <= centerY + 151)
			{
				bgrPixel[0] = 0;
				bgrPixel[1] = 0;
				bgrPixel[2] = 0;

				image.at<Vec3b>(y, x) = bgrPixel;
			}

		}

	}

	return;
}

Vec3b avgColor(Mat image)
{
	int centerY = image.rows / 2;
	int centerX = image.cols / 2;

	int avgB = 0;
	int avgG = 0;
	int avgR = 0;

	int counter = 0;

	for (int y = 0; y < image.rows; y++) // y = rows
	{

		for (int x = 0; x < image.cols; x++) // x = cols
		{
			Vec3b bgrPixel = image.at<Vec3b>(y, x);

			// Center Color
			if (x > centerX - 48 && x < centerX + 48 &&
				y > centerY - 48 && y < centerY + 48)
			{
				counter++;
				avgB += bgrPixel[0]; // Blue
				avgG += bgrPixel[1]; // Green
				avgR += bgrPixel[2]; // Red
				image.at<Vec3b>(y, x) = bgrPixel;
			}
		}
	}

	avgB = avgB / counter;
	avgG = avgG / counter;
	avgR = avgR / counter;

	Vec3b c2Color(avgB, avgG, avgR);

	vector<Vec3b> avgColors



	return avgColors;
}

void drawAvgColor(Mat image, vector<Vec3b> avgColors)
{
	int maxY = image.rows;
	int maxX = image.cols;

	for (int y = 0; y < image.rows; y++) // y = rows
	{

		for (int x = 0; x < image.cols; x++) // x = cols
		{
			// Center Color
			if (x > maxX - 150 && x < maxX - 50 &&
				y > 20 && y < 120)
			{
				image.at<Vec3b>(y, x) = avgColors[4];
			}


		}

	}

}

int main(int argc, char** argv)
{
	VideoCapture cap;
	cap.open(1);
	if (!cap.isOpened())
	{
		printf("--(!)Error opening video capture\n");
		return -3;
	}


	//Mat image = imread(".\\edgecube.jpeg");
	//Mat image;
	//Rect rec(300, 200, 100, 100);

	//if (!image.data)
	//{
	//	return -1;
	//}

	Mat image;

	namedWindow("Image", CV_WINDOW_AUTOSIZE);

	//	cvtColor(image, image, CV_BGR2GRAY);

	//cap.read(image);

	//cout << "rows: " << image.rows << endl;
	//cout << "cols: " << image.cols << endl;


	while (1)
	{
		//Mat image;
		//cap >> image;

		cap.read(image);
		
		// 
		drawLines(image);
		drawAvgColor(image, avgColor(image));
		imshow("Image", image);
		waitKey(30);
	}


	//while (1)
	//{
	//	cap.read(image);

	//	//rectangle(image, rec.tl(), rec.br(), Scalar(0, 0, 255), 2, 8, 0);
	//	imshow("Image", image);

	//	waitKey(30);
	//}
	return 0;
}