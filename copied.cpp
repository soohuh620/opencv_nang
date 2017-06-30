#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

#define SCALE 0.2
#define NUM 6
#define BINS 8

/**
* @function main
*/
int main(int argc, char** argv)
{
	/*Mat src, dst;

	/// Load image
	src = imread("../image/ILSVRC2012_val_00000023.JPEG", IMREAD_COLOR);
	imshow("yeah", src);

	if (!src.data)
	{
		return -1;
	}

	/// Separate the image in 3 places ( B, G and R )
	vector<Mat> bgr_planes;
	split(src, bgr_planes);

	/// Establish the number of bins
	int histSize = 256;

	/// Set the ranges ( for B,G,R) )
	float range[] = { 0, 256 };
	const float* histRange = { range };

	bool uniform = true; bool accumulate = false;

	Mat b_hist, g_hist, r_hist;

	/// Compute the histograms:
	calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);

	// Draw the histograms for B, G and R
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound((double)hist_w / histSize);

	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

	/// Normalize the result to [ 0, histImage.rows ]
	normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	/// Draw for each channel
	for (int i = 1; i < histSize; i++)
	{
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(b_hist.at<float>(i))),
			Scalar(255, 0, 0), 2, 8, 0);
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(g_hist.at<float>(i))),
			Scalar(0, 255, 0), 2, 8, 0);
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(r_hist.at<float>(i))),
			Scalar(0, 0, 255), 2, 8, 0);
	}

	/// Display
	namedWindow("calcHist Demo", WINDOW_AUTOSIZE);
	imshow("calcHist Demo", histImage);

	Mat result,src2;
	src2 = imread("../image/ILSVRC2012_val_00000023.JPEG", IMREAD_COLOR);
	add(src2, src, result);
	imshow("yeah2", result);

	waitKey(0);

	return 0;*/

	// the names of images to be compared each other
	string imgNames[NUM] = { "ILSVRC2012_val_00000006.JPEG", "ILSVRC2012_val_00000018.JPEG", "ILSVRC2012_val_00000021.JPEG",
		"ILSVRC2012_val_00000023.JPEG","ILSVRC2012_val_00000032.JPEG","ILSVRC2012_val_00000031.JPEG"};


	// read all images and convert to HLS format
	Mat imgs[NUM];
	Mat imgsHLS[NUM];
	for (int i = 0; i < NUM; i++)
	{
		imgs[i] = imread(imgNames[i], IMREAD_COLOR);
		if (imgs[i].data == 0)
		{
			cout << "Unable to read " << imgNames[i] << endl;
			return 0;
		}
		cvtColor(imgs[i], imgsHLS[i], COLOR_BGR2HLS);
	}

	//cout << "Succeeded to read all images" << endl;

	// compute 3D histogram
	Mat histogram[NUM];
	

	int channel_numbers[] = { 0, 1, 2 };
	for (int i = 0; i < NUM; i++)
	{
		int* number_bins = new int[imgsHLS[i].channels()];
		for (int ch = 0; ch < imgsHLS[i].channels(); ch++)
		{
			number_bins[ch] = BINS;
		}
		float ch_range[] = { 0.0, 255.0 };
		const float *channel_ranges[] = { ch_range, ch_range, ch_range };
		calcHist(&imgsHLS[i], 1, channel_numbers, Mat(), histogram[i], imgsHLS[i].channels(), number_bins, channel_ranges);
		normalize(histogram[i], histogram[i], 1.0);
		//imshow("tt", histogram[i]);
		//cout << histogram << endl;
		//print the histogram to text (HOW)_170630
	}


	cout << "Image Comparison by HISTCMP_CORREL   " << endl;
	for (int i = 0; i < NUM; i++)
	{
		for (int j = i + 1; j < NUM; j++)
		{
			printf("--------------------------------------------------------------------------------------\n");
			double matching_score = compareHist(histogram[i], histogram[j], HISTCMP_CORREL);
			cout << imgNames[i] << "-" << imgNames[j] << ", " << matching_score << endl;
			matching_score = compareHist(histogram[i], histogram[j], HISTCMP_CHISQR);
			cout << imgNames[i] << "-" << imgNames[j] << ", " << matching_score << endl;
			matching_score = compareHist(histogram[i], histogram[j], CV_COMP_INTERSECT);
			cout << imgNames[i] << "-" << imgNames[j] << ", " << matching_score << endl;
			matching_score = compareHist(histogram[i], histogram[j], CV_COMP_BHATTACHARYYA);
			cout << imgNames[i] << "-" << imgNames[j] << ", " << matching_score << endl;
			printf("--------------------------------------------------------------------------------------\n");
		}

	}

	return 0;
}

//이미지를 넣으면 그와 유사한  top 10 출력 (이미지)
