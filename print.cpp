#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

#define NUM 100	//total number of images
#define BINS 8

int main(){
	Mat histogram(256, 1, CV_32F);

	/*
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
	*/

	Mat imgsHLS;
	Mat img = imread("ILSVRC2012_val_00000006.JPEG", IMREAD_COLOR);
	cvtColor(img, imgsHLS, COLOR_BGR2HLS);

	int channel_numbers[] = { 0, 1, 2 };
	
	int* number_bins = new int[imgsHLS.channels()];
	for (int ch = 0; ch < imgsHLS.channels(); ch++)
	{
		number_bins[ch] = BINS;
	}
	float ch_range[] = { 0.0, 255.0 };
	const float *channel_ranges[] = { ch_range, ch_range, ch_range };
	calcHist(&imgsHLS, 1, channel_numbers, Mat(), histogram, imgsHLS.channels(), number_bins, channel_ranges);
	normalize(histogram, histogram, 1.0);
		
	for (int i = 0; i < 256; i++){
		cout << "b"<< histogram.at<float>(0, i)<<",";
		cout << "g"<< histogram.at<float>(1, i)<<",";
		cout << "r" << histogram.at<float>(2, i) << endl;
	}
		//print the histogram to text (HOW)_170630
	
}