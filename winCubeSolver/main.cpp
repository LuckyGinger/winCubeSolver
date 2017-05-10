#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
//#include <opencv2/videoio.hpp>
//#include <opencv2/objdetect.hpp>

#define X33SIDE 9
#define BGR 3
#define CLRSIZE 96 * 96

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

int **calcAvgColor(int **avg, int c = CLRSIZE)
{
	for (int i = 0; i < X33SIDE; i++)
		for (int j = 0; j < BGR; j++)
		{
			//cout << "[" << i << "][" << j << "]" << avg[i][j] << endl;
			//cout << "[" << i << "][" << j << "]" << avg[i][j] / c << endl;
			avg[i][j] = avg[i][j] / c;

		}

	return avg;
}

Vec3b *avgColor(Mat image)
{
	int centerY = image.rows / 2;
	int centerX = image.cols / 2;

	// Allocate memory for multi array
	int **avgColorArray = new int *[X33SIDE];
	for (int i = 0; i < X33SIDE; ++i)
		avgColorArray[i] = new int[BGR];

	// Assign Zeros to all places
	for (int i = 0; i < X33SIDE; i++)
		for (int j = 0; j < BGR; j++)
			avgColorArray[i][j] = 0;

	int counter = 0;
	for (int y = 0; y < image.rows; y++) // y = rows  
	{

		for (int x = 0; x < image.cols; x++) // x = cols
		{

			// Top Left Color
			if (x > centerX - 148 && x < centerX - 52 &&
				y > centerY - 148 && y < centerY - 52)
			{
				Vec3b t1bgrPixel = image.at<Vec3b>(y, x);
				
				avgColorArray[0][0] += t1bgrPixel[0]; // Blue
				avgColorArray[0][1] += t1bgrPixel[1]; // Green
				avgColorArray[0][2] += t1bgrPixel[2]; // Red
				image.at<Vec3b>(y, x) = t1bgrPixel;
			}
			// Center Color
			if (x > centerX - 48 && x < centerX + 48 &&
				y > centerY - 48 && y < centerY + 48)
			{
				Vec3b bgrPixel = image.at<Vec3b>(y, x);

				counter++;
				avgColorArray[4][0] += bgrPixel[0]; // Blue
				avgColorArray[4][1] += bgrPixel[1]; // Green
				avgColorArray[4][2] += bgrPixel[2]; // Red
				image.at<Vec3b>(y, x) = bgrPixel;
			}
		}
	}

	avgColorArray = calcAvgColor(avgColorArray);

	//for (int i = 0; i < X33SIDE; i++)
	//{
	//	for (int j = 0; j < BGR; j++)
	//	{
	//		cout << "[" << i << "][" << j << "]" <<  avgColorArray[i][j] << endl;
	//	}
	//	cout << "block" << endl;
	//}


	//avgColorArray[4][0] = avgColorArray[4][0] / counter;
	//avgColorArray[4][1] = avgColorArray[4][1] / counter;
	//avgColorArray[4][2] = avgColorArray[4][2] / counter;

	Vec3b c_t1(avgColorArray[0][0], avgColorArray[0][1], avgColorArray[0][2]);
	Vec3b c_t2(avgColorArray[1][0], avgColorArray[1][1], avgColorArray[1][2]);
	Vec3b c_t3(avgColorArray[2][0], avgColorArray[2][1], avgColorArray[2][2]);
	Vec3b c_c1(avgColorArray[3][0], avgColorArray[3][1], avgColorArray[3][2]);
	Vec3b c_c2(avgColorArray[4][0], avgColorArray[4][1], avgColorArray[4][2]);
	Vec3b c_c3(avgColorArray[5][0], avgColorArray[5][1], avgColorArray[5][2]);
	Vec3b c_b1(avgColorArray[6][0], avgColorArray[6][1], avgColorArray[6][2]);
	Vec3b c_b2(avgColorArray[7][0], avgColorArray[7][1], avgColorArray[7][2]);
	Vec3b c_b3(avgColorArray[8][0], avgColorArray[8][1], avgColorArray[8][2]);

	Vec3b avgColors[9] = { c_t1, c_t2, c_t3,
						   c_c1, c_c2, c_c3,
						   c_b1, c_b2, c_b3 };
	

	// dealocate memory
	for (int i = 0; i < X33SIDE; i++)
		delete[] avgColorArray[i];
	delete[] avgColorArray;

	//for (int i = 0; i < X33SIDE; i++)
	//{
	//	cout << avgColors[i] << endl;
	//}

	return avgColors;
}

void drawAvgColor(Mat image, Vec3b avgColors[])
{
	int maxY = image.rows;
	int maxX = image.cols;

	for (int y = 0; y < image.rows; y++) // y = rows
	{

		for (int x = 0; x < image.cols; x++) // x = cols
		{
			// Top Left Color
			if (x > maxX - 150 && x < maxX - 120 &&
				y > 20 && y < 50)
			{
				image.at<Vec3b>(y, x) = avgColors[0];
			}
			// Top Middle Color
			else if (x > maxX - 120 && x < maxX - 90 &&
				y > 20 && y < 50)
			{
				image.at<Vec3b>(y, x) = avgColors[1];
			}
			// Top Right Color
			else if (x > maxX - 90 && x < maxX - 60 &&
				y > 20 && y < 50)
			{
				image.at<Vec3b>(y, x) = avgColors[2];
			}
			// Center Left Color
			else if (x > maxX - 150 && x < maxX - 120 &&
				y > 50 && y < 80)
			{
				image.at<Vec3b>(y, x) = avgColors[3];
			}
			// Center Middle Color
			else if (x > maxX - 120 && x < maxX - 90 &&
				y > 50 && y < 80)
			{
				/*cout << "second" << endl;
				for (int i = 0; i < X33SIDE; i++)
				{
					cout << i << avgColors[i] << endl;
				}*/
				image.at<Vec3b>(y, x) = avgColors[4];
			}
			// Center Right Color
			else if (x > maxX - 90 && x < maxX - 60 &&
				y > 50 && y < 80)
			{
				image.at<Vec3b>(y, x) = avgColors[5];
			}
			// Bottom Left Color
			else if (x > maxX - 150 && x < maxX - 120 &&
				y > 80 && y < 110)
			{
				image.at<Vec3b>(y, x) = avgColors[6];
			}
			// Bottom Middle Color
			else if (x > maxX - 120 && x < maxX - 90 &&
				y > 80 && y < 110)
			{
				image.at<Vec3b>(y, x) = avgColors[7];
			}
			// Bottom Right Color
			else if (x > maxX - 90 && x < maxX - 60 &&
				y > 80 && y < 110)
			{
				image.at<Vec3b>(y, x) = avgColors[8];
			}

		}

	}

}

int main(int argc, char** argv)
{
	Mat image;
	VideoCapture cap;
	Vec3b *avgg = new Vec3b[X33SIDE];
	//for (int i = 0; i < X33SIDE; i++)
	//cout << avgg[i] << endl;

	namedWindow("Image", CV_WINDOW_AUTOSIZE);

	cap.open(0);
	if (!cap.isOpened())
	{
		printf("--(!)Error opening video capture\n");
		return -3;
	}

	while (1)
	{
		cap.read(image);
		
		drawLines(image);

		// get average color array
		for (int i = 0; i < X33SIDE; i++)
		{
			avgg[i] = avgColor(image)[i];

		//for (int i = 0; i < X33SIDE; i++)
		//{
		//	cout << avgg[i] << endl;
		//}


			//cout << i << avgg[i] << endl;
		}
		//cout << "block" << endl;
		drawAvgColor(image, avgg);
		imshow("Image", image);
		waitKey(10);
	}


	//while (1)
	//{
	//	cap.read(image);

	//	//rectangle(image, rec.tl(), rec.br(), Scalar(0, 0, 255), 2, 8, 0);
	//	imshow("Image", image);

	//	waitKey(30);
	//}
	delete[] avgg;	
	return 0;
}