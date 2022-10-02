#include "cv.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

int rotation = 90, gamma = 10;
unsigned char pix[256];



int main() {
	Mat image = imread("moon.png", 0);
	Mat moon_filt = image.clone();
	Mat moon_filt1 = image.clone();
	if (image.empty()) {
		cerr << "image reading failed" << endl;
		exit(-1);
	}
	
	//Blurring the image
	Mat blurred_roi, sub_mask, unsharp_mask;
	blur(moon_filt1, blurred_roi, Size(3, 3));
	//Obtaining the mask
	sub_mask = moon_filt1 - blurred_roi;
	//performing the sharpening
	unsharp_mask = moon_filt1 + 6 * sub_mask;
	//Restoring the initial values of pixels to the left-half of the sharpened image. 
	//This will result in having the sharpening only on the right-half side of the original image
	for (int i = 0; i < moon_filt.rows; i++) {
		for (int j = 0; j < moon_filt.cols/2; j++) {
			unsharp_mask.at<uchar>(i, j) = moon_filt.at<uchar>(i, j);
		}
	}
	image = imread("saltnpepper.png", 0);
	Mat salt = image.clone();
	Mat salt_filt = image.clone();
	if (image.empty()) {
		cerr << "image reading failed" << endl;
		exit(-1);
	}
	medianBlur(salt, salt_filt,9);
	//Restoring the initial values of pixels to the right-half of the sharpened image. 
	//This will result in having the filtering only on the left-half side of the original image
	for (int i = 0; i < salt.rows; i++) {
		for (int j = salt.cols / 2; j < salt.cols; j++) {
			salt_filt.at<uchar>(i, j) = salt.at<uchar>(i, j);
		}
	}
	//Showing the results in 4 windows
	imshow("moon", moon_filt);
	imshow("moon_filtered", unsharp_mask);
	imshow("saltnpepper", salt);
	imshow("saltnpepper_filtered", salt_filt);

	waitKey(0);
}