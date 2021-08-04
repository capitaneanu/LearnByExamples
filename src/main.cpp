/**
* @file		main.cpp
* @author	Sunghwan Lim <shjlim@jhu.edu>
* @version	1.0
*
* @section	DESCRIPTION
*
* Introduction to Augmented Reality and Virtual Reality
*/
#include <iostream>
#include <stdio.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "MarkerTracker.h"

using namespace std;
using namespace cv;

bool use_webcam = false;

void thresholdTrackbarHandler( int pos, void* slider_value ) {
    *( (int*)slider_value ) = pos;
}
void minMkrSizeTrackbarHandler(int pos, void* slider_value) {
	*((int*)slider_value) = pos;
}
void maxMkrSizeTrackbarHandler(int pos, void* slider_value) {
	*((int*)slider_value) = pos;
}

void initVideoStream( cv::VideoCapture &cap ) {
    if( cap.isOpened() )
        cap.release();

    if (use_webcam)
        cap.open(0); // open the default camera
    else {
        cap.open("MarkerMovie.mpg");
        if ( cap.isOpened()==false ) {
            std::cout << "No video file found. Exiting."      << std::endl;
            exit(0);
        }
    }
}

int main(int ac, char** av)
{
	/// For OpenCV tutorial
	// Video stream
    VideoCapture cap;
	// Number of windows
	const int nWin = 11;
	// Window names
	const string kWinName[nWin] = { "Original Image", "Step1", "Step2",
									"Step3", "Step4", "Step5", "Step6",
									"Step7", "Step8", "Step9", "Step10" };
	
	// Prepare cv windows
	for (int i = 0; i < nWin; i++)
		namedWindow(kWinName[i], WINDOW_AUTOSIZE);

	// Arrange the positions of windows
	for (int i = 0; i < nWin; i++)
		moveWindow(kWinName[i], (i%6)*320, (int)floor(i/6)*320);

	// Initialize video stream
    initVideoStream(cap);
	    
	// Create slider for Step1
    int slider_value = 130;
    createTrackbar( "Threshold", "Step1", &slider_value, 255, thresholdTrackbarHandler, &slider_value);

	// Create slider for Step3
	int minMkrSize = 40, maxMkrSize = 200;
	createTrackbar("Min Size", "Step3", &minMkrSize, 200, minMkrSizeTrackbarHandler, &minMkrSize);
	createTrackbar("Max Size", "Step3", &maxMkrSize, 400, maxMkrSizeTrackbarHandler, &maxMkrSize);
       
	MarkerTracker mt;	// MarkerTracker object
    Mat img_bgr;		// Original image
	Mat img_gray;		// Gray scale image
    Mat img_mono;		// Thresholed image
	
    while (1) {
        cap >> img_bgr;
		
        if(img_bgr.empty()){
            cout << "Could not query frame. Trying to reinitialize." << endl;
            initVideoStream(cap);
            waitKey(1000); /// Wait for one sec.			
            continue;
        }
		imshow(kWinName[0], img_bgr);

		/**********************************************************/
		// Step1
        // Simple BW thresholding		
		img_mono = mt.Threshold(img_bgr, slider_value, img_gray);
		imshow(kWinName[1], img_mono);
		/**********************************************************/
		
		/**********************************************************/
		// Step2
		// Find contours
		Vec2Pt contours;
		imshow(kWinName[2], mt.FindContours(img_mono, contours));
		/**********************************************************/

		/**********************************************************/
		// Step3
		// Extract marker contours
		Vec2Pt marker_corners;	
		imshow(kWinName[3], mt.ExtractMarkerContours(contours, minMkrSize, maxMkrSize, 4, marker_corners));
		/**********************************************************/

		/**********************************************************/
		// Step4
		// Draw contours of markers with lines and circles		
		imshow(kWinName[4], mt.DrawMarkerContours(marker_corners));
		/**********************************************************/

		/**********************************************************/
		// Step5 & Step6
		// Find six equal distant points on each edge e. 
		// Then, sample 6 3-pixel wide stripes with length h.
		// Finally, apply the y-sobel operator on each stripe.
		Vec3Pt2f equalDistPts;
		Vec3Mat stripes;
		Vec3Mat stripes_sobel;
		imshow(kWinName[5], mt.SampleStripesFromEdges(img_bgr, marker_corners, equalDistPts, stripes, stripes_sobel));

		// Show stripes
		// In order to save computation time, show only the stripes of the first edge of the first marker.
		if ((int)stripes.size()) {			
			Mat dst_strips, dst_strips_sobel;
			int width = stripes[0][0][0].cols;
			int length = stripes[0][0][0].rows;
			// Make stripes bigger to check the results (x40)
			resize(stripes[0][0][0], dst_strips, Size(width * 40, length * 40),0,0,INTER_NEAREST);
			resize(stripes_sobel[0][0][0], dst_strips_sobel, Size(width * 40, length * 40), 0, 0, INTER_NEAREST);
			imshow(kWinName[6], dst_strips);
			imshow(kWinName[7], dst_strips_sobel);
		}				
		/**********************************************************/

		/**********************************************************/
		// Step7
		// Fit a parabola y=ax^2+bx+c to the stripe, then compute the extremum of 
		// the parabola to find the sub-pixel accurate location of the edge.
		Vec3Pt2f subPoints;
		mt.FindAccurateEdges(marker_corners, equalDistPts, stripes_sobel, subPoints);
		/**********************************************************/

		/**********************************************************/
		// Step8
		// Fit lines to the 6-pint sets on the edges of the markers.
		Vec2Vec4f edgeLines;		
		imshow(kWinName[8], mt.FitLines(subPoints, edgeLines));
		/**********************************************************/

		/**********************************************************/
		// Step9
		// Find accurate corners for the marekr.
		Vec2Pt corners;		
		imshow(kWinName[9], mt.FindAccurateCorners(edgeLines, corners));
		/**********************************************************/

		/**********************************************************/
		// Step10
		// Draw the final lines and points on the image.
		Mat img_stripes = mt.ShowStripesLocations(img_bgr, marker_corners, equalDistPts, stripes_sobel);
		imshow(kWinName[10], mt.ShowFinalLinesAndPoints(img_stripes, edgeLines, corners));		
		/**********************************************************/

		// Clear containers
		equalDistPts.clear();
		stripes.clear();
		stripes_sobel.clear();
		contours.clear();
		corners.clear();
		subPoints.clear();	
		edgeLines.clear();
		marker_corners.clear();
		
        char key = waitKey (10);
        if (key == 27) break;			// Quit the program when 'ESC' button pressed
		if (key == 112) waitKey(0);  	// Pause the program when 'p' button pressed
    } // end of main loop
	
	// Destroy all windows
	destroyAllWindows();    
}
