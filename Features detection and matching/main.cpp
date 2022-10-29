#include "cv.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
//#include <glob.h>

using namespace std;
using namespace cv;

int main() {
	Mat frame,img_query, descriptors1, descriptors2, imgMatches;
	string query_name, best_matching, file;
	string folder = "C:/Users/HP/source/repos/H2/H2/assignment7/DBs/Handong*_1.jpg";
	vector <String> filenames, train_images;
	vector< DMatch > goodMatches;
	Ptr<ORB> orbF = ORB::create(1000);
	vector<KeyPoint> keypoints1, keypoints2;
	vector <vector<KeyPoint>> train_keypoints;
	vector< vector< DMatch> > matches, train_goodMatches; //DMatch is descriptor match
	
	BFMatcher matcher(NORM_HAMMING);
	int i, k;
	float nndr;


	//Asking the query image name
	cout << "Enter the name of your query image..." << endl;
	cin >> query_name;

	img_query= imread("C:/Users/HP/source/repos/H2/H2/assignment7/query_image/"+query_name + ".jpg");
	if (img_query.empty()) {
		cout << "There is no such image file !" << endl;
		return -1;
	}
	resize(img_query, img_query, Size(640, 480));
	orbF->detectAndCompute(img_query, noArray(), keypoints1, descriptors1);

	//loading the DBs images 
	/*for (int count = 0; count < 10; count++) {
		file = folder + to_string(count + 1) + "_1.jpg";
		
		frame = imread(file);
		if (frame.empty()) {
			cout << "There is no such image file in DBs!" << endl;
			break;
		}

		resize(frame, frame, Size(640, 480));
		orbF->detectAndCompute(frame, noArray(), keypoints2, descriptors2);

		//KNN Matching(knearest neighbor matching)
		//Find best and second	best matches
		k = 2;
		matcher.knnMatch(descriptors1, descriptors2, matches, k);
		
		// Find out the best match is definitely better than the second best match
		nndr = 0.6f;
		
		for (i = 0; i < matches.size(); i++) {
			if (matches.at(i).size() == 2 && matches.at(i).at(0).distance <= nndr * matches.at(i).at(1).distance) {
				goodMatches.push_back(matches[i][0]);
				//file = folder + to_string(i+1) + "_1.jpg";
				
				
			}
		}
		cout << goodMatches[i] << endl;
		//goodMatchList[goodMatches.size()];
	}
	//Draws the found matches of keypoints from two images.
	drawMatches(img_query, keypoints1, frame, keypoints2, goodMatches, imgMatches,
		Scalar::all(1), Scalar(1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
	if (goodMatches.size() < 4) {
		cout << "Matching failed" << endl;
		return 0;
	}*/
	int count = 0;
	glob(folder,filenames,false);
	nndr = 0.6f;
	for (int count = 0; count < filenames.size(); count++) {
		frame = imread(filenames[count]);

		if (frame.empty()) {
			cout << "There is no such image file in DBs!" << endl;
			break;
		}
		resize(frame, frame, Size(640, 480));
		//Compute ORB Features
		orbF->detectAndCompute(frame, noArray(), keypoints2, descriptors2);

		//KNN Matching(knearest neighbor matching)
		//Find best and second	best matches
		k = 2;
		matches.clear();
		matcher.knnMatch(descriptors1, descriptors2, matches, k);

		// Find out the best match is definitely better than the second best match
		
		goodMatches.clear();
		for (i = 0; i < matches.size(); i++) {
			if (matches.at(i).size() == 2 && matches.at(i).at(0).distance <= nndr * matches.at(i).at(1).distance) {
				goodMatches.push_back(matches[i][0]);
			}
		}
		//Saving the keypoints and the matching points of the current train image
		train_goodMatches.push_back(goodMatches);
		train_keypoints.push_back(keypoints2);
		cout << "Image number "+to_string(count+1)+", Matching points = " + to_string(goodMatches.size()) << endl;

	}
	//Selecting the best matching image, that is the images with the most matching points
	int max = 0, final_index=0;	
	for (int j = 0; j < train_goodMatches.size(); j++) {
		if (train_goodMatches[j].size()>max)  {
			max = train_goodMatches[j].size();
			final_index = j;
		}
		
	}

	Mat final_img = imread(filenames[final_index]);
	resize(final_img, final_img, Size(640, 480));

	//Draws the found matches of keypoints from best training image.
	drawMatches(img_query, keypoints1, final_img, train_keypoints[final_index], train_goodMatches[final_index], imgMatches,
		Scalar::all(-1), Scalar(1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
	if (train_goodMatches[final_index].size() < 4) {
		cout << "Matching failed" << endl;
		return 0;
	}
		
	imshow("Query image", img_query);
	imshow("Best Matching", imgMatches);
	waitKey(0);
	return(0);
}
	
