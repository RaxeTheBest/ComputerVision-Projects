#include "cv.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

void PrintLinesInImage(Mat input_mat, vector<Vec2f> linesl, vector<Vec2f> linesr, int x, int y)
{
	float rho=0, theta=0, a=0, b=0, x0=0, y0=0, avg_rho=0, avg_theta=0;
	Point p1, p2,p3,p4;


	for (int i = 0; i < linesl.size(); i++) {
		//rho = linesl[i][0];
		avg_rho = avg_rho + linesl[i][0];

		//theta = linesl[i][1];
		avg_theta = avg_theta + linesl[i][1];
	}
	avg_rho = cvRound(avg_rho / linesl.size());
	avg_theta = cvRound(avg_theta / linesl.size());
	a = cos(avg_theta);
	b = sin(avg_theta);
	x0 = a * avg_rho;
	y0 = b * avg_rho;
	p1 = Point(cvRound(x + x0 + 1000 * (-b)), cvRound(y + y0 + 1000 * a));
	p2 = Point(cvRound(x + x0 - 1000 * (-b)), cvRound(y + y0 - 1000 * a));
	

	rho = 0, theta = 0, a = 0, b = 0, x0 = 0, y0 = 0, avg_rho = 0, avg_theta = 0;
	for (int j = 0; j < linesr.size(); j++) {
		//rho = linesr[j][0];
		avg_rho = avg_rho + linesr[j][0];

		//theta = linesr[j][1];
		avg_theta = avg_theta + linesr[j][1];
	}
	avg_rho = avg_rho / linesr.size();
	avg_theta = avg_theta / linesr.size();
	a = cos(avg_theta);
	b = sin(avg_theta);
	x0 = a * avg_rho;
	y0 = b * avg_rho;
	p3 = Point(cvRound(x+x0 + 1000 * (-b)) , cvRound(y+y0 + 1000 * a));
	p4 = Point(cvRound(x+x0 - 1000 * (-b)) , cvRound(y+y0 - 1000 * a));

	line(input_mat, p1, p2, Scalar(0, 0, 255), 3, 2);
	line(input_mat, p3, p4, Scalar(0, 0, 255), 3, 2);
	

}
int main() {
	Mat frame;
	VideoCapture capture;
	int fps, delay, frame_pos = 0, minutes;
	float min, max;

	if (capture.open("Road.mp4") == 0) {
		cout << "no such file!" << endl;
		waitKey(0);
	}
	fps = capture.get(CAP_PROP_FPS);
	minutes = capture.get(CAP_PROP_POS_MSEC);

	Rect left(200, 400, 400, 200), right(600, 400, 400, 200);
	Mat left_roi, right_roi, left_gray, right_gray;
	delay = 1000 / fps;

	vector<Vec2f> linesl,linesr;
	while (1) {
		capture >> frame;
		
		//Canny for left ROI
		left_roi = frame(left);
		cvtColor(left_roi, left_gray, CV_BGR2GRAY);
		blur(left_gray, left_gray, Size(5, 5));
		Canny(left_gray, left_gray, 10, 60,3);

		//Canny for right ROI
		right_roi = frame(right);
		cvtColor(right_roi, right_gray, CV_BGR2GRAY);
		blur(right_gray, right_gray, Size(5, 5));
		Canny(right_gray, right_gray, 10, 60,3);

		//Line filtering for the left  ROI
		min = 30 * (CV_PI / 180); 
		max = 60 * (CV_PI / 180);
		HoughLines(left_gray, linesl, 1, CV_PI / 180, 97, 0, 0, min, max);
		//Line filtering for the right ROI
		min = 120 * (CV_PI / 180);
		max = 150 * (CV_PI / 180);
		HoughLines(right_gray, linesr, 1, CV_PI / 180, 34, 0, 0, min, max);

		//Printing the lines of the frame for left and right ROI
		PrintLinesInImage(frame, linesl,linesr, 400, 200);

		if (frame.empty()) {
			cout << "End of the video" << endl;
			break;
		}

		imshow("video", frame);
		namedWindow("Left canny");
		moveWindow("Left canny", 200, 0);
		imshow("Left canny", left_gray);
		namedWindow("Right canny");
		moveWindow("Right canny", 600, 0);
		imshow("Right canny", right_gray);

		//Displaying the video during 20 seconds
		frame_pos = capture.get(CAP_PROP_POS_MSEC);
		if (frame_pos > 20001) break;
		waitKey(delay);
	}
	return(0);
}