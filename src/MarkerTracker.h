#pragma once

#include <iostream>
#include <stdio.h>
#include <Eigen/Dense>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

// Type definitions of containers
typedef vector<Mat>						Vec1Mat;
typedef vector<vector<vector<Mat>>>		Vec3Mat;
typedef vector<Point>					Vec1Pt;
typedef vector<Point2f>					Vec1Pt2f;
typedef vector<vector<Point>>			Vec2Pt;
typedef vector<vector<Point2f>>			Vec2Pt2f;
typedef vector<vector<vector<Point>>>	Vec3Pt;
typedef vector<vector<vector<Point2f>>> Vec3Pt2f;
typedef vector<vector<Vec4f>>			Vec2Vec4f;

class MarkerTracker
{
public:
	MarkerTracker();
	~MarkerTracker();

	// Attributes
private:
	/// Image size
	Size m_imageSize;
	/// Original image
	Mat m_originalImage;
	/// Image data for the thresholded image
	Mat m_thresholdOutput;
	/// Image data for the found contours
	Mat m_contourOutput;
	/// Image data for the extracted marker contours
	Mat m_markerContourOutput1;
	/// Image data for the extracted marker contours
	Mat m_markerContourOutput2;
	/// Image data for the sampled stripes from edges
	Mat m_sampledStripes;
	/// Image data for the line fittings
	Mat m_lineOutput;
	/// Image data for the accurate corners
	Mat m_cornerOutput;
	/// Image data for the final lines and points
	Mat m_finalLinesAndPoints;
	/// Image data for the locations of the stripes
	Mat m_stripesLocations;
public:

	// Operations
private:	
	/**
	*	\brief	Sample sub-pixel image intensity
	*	\param[in]	src				Source image
	*	\param[in]	p				Sapling position
	*	\param[out]	return			Interpolated image intensity	
	*/
	int SampleSubPix(const Mat src, const Point2f p);	
	/**
	*	\brief	Sample equal distance points from an edge
	*	\param[in]	edgeEndpoint1	First end-point of edge
	*	\param[in]	edgeEndpoint2	Second end-point of edge
	*	\param[in]	numPoints		Number of points
	*	\param[out]	outPutPoints	Equal distance points
	*	\param[out]	directionVec	Edge direction vector
	*/
	double SampleEqualDistPoints(Point edgeEndpoint1, Point edgeEndpoint2, int numPoints, Vec1Pt2f& outPutPoints, Point2f& directionVec);	
	/**
	*	\brief	Sample strips
	*	\param[in]	srcImg			Source image
	*	\param[in]	cenPts			Center points of stripes
	*	\param[in]	direction		Direction vector of stripes
	*	\param[in]	width			Width of stripes
	*	\param[in]	length			Length of stripes
	*	\param[out]	strips			Sampled stripe images
	*	\param[out]	strips_sobel	Stripe images obtained after applying y-Sobel operator
	*/
	void SampleStrips(Mat srcImg, Vec1Pt2f cenPts, Point2f direction, int width, int length, Vec1Mat& strips, Vec1Mat& strips_sobel);	
	/**
	*	\brief	Find the stripe's maximum and its two neighbor pixels
	*	\param[in]	srcImg			Source image (Stripe images)
	*	\param[out]	result			Indexes of the stripe's maximum and its two neighbor pixels
	*	\param[out]	values			Image intensity values of the stripe's maximum and its two neighbor pixels
	*/
	void FindStripesMaximum(Mat srcImg, Vec1Pt& result, vector<unsigned int>& values);	
	/**
	*	\brief	Fit a parabola to the stripes maximum and its two neighbor pixels.
	*	\param[in]	values			Image intensity values of the stripe's maximum and its two neighbor pixels
	*	\param[out]	offset			Offset value of the extremum from the position of the stripe's maximum
	*/
	void FitParabola(vector<unsigned int> values, double& offset);	
	/**
	*	\brief	Compute a sub-pixel accurate position of edge
	*	\param[in]	edgeDirection	Direction vector of edge
	*	\param[in]	cenStripe		Center point of stripe
	*	\param[in]	maxPixel		Indexes of the stripe's maximum
	*	\param[in]	offset			Offset value of the extremum from the position of the stripe's maximum
	*	\param[in]	stripeLength	Length of stripes
	*	\param[out]	subPoint		Sub-pixel accurate point	
	*/
	void ComputeSubAccuratePoint(Point2f edgeDirection, Point2f cenStripe, Point maxPixel, double offset, int stripeLength, Point2f& subPoint);
public:	
	/**
	*	\brief	Q3-(a) Simple BW thresholding
	*	\param[in]	srcImage		Source image
	*	\param[in]	thValue			Threshold value
	*	\param[out]	grayImage		Black and white image
	*	\param[out]	return			Output image
	*/
	Mat Threshold(Mat srcImage, double thValue, Mat& grayImage);	
	/**
	*	\brief	Q3-(b) Find contours from the BW image
	*	\param[in]	srcImage		Source image	
	*	\param[out]	contours		Contours
	*	\param[out]	return			Output image
	*/
	Mat FindContours(Mat srcImage, Vec2Pt& contours);	
	/**
	*	\brief	Q3-(c) Extract the contours of markers
	*	\param[in]	contours		Contours
	*	\param[in]	minSize			Minimum size of polygon 
	*	\param[in]	maxSize			Maximum size of polygon
	*	\param[in]	numCorners		Number of corners of polygon
	*	\param[out]	markerContours	Contours of AR marker
	*	\param[out]	return			Output image
	*/
	Mat ExtractMarkerContours(Vec2Pt contours, int minSize, int maxSize, int numCorners, Vec2Pt& markerContours);	
	/**
	*	\brief	Q3-(d) Draw contours of marker with lines and circles
	*	\param[in]	contours		Contours
	*	\param[in]	edgeColor		Color of marker edges
	*	\param[in]	cornerColor		Color of marker corners
	*	\param[out]	return			Ouput image
	*/
	Mat DrawMarkerContours(Vec2Pt contours, Scalar edgeColor = Scalar(255, 0, 255), Scalar cornerColor = Scalar(0, 255, 255));
	/**
	*	\brief	Q3-(e)&(f) Sample stripes from edges and apply y-Sobel operator
	*	\param[in]	srcImage		Source image
	*	\param[in]	markerEdges		Marker edges (corners)
	*	\param[out]	equalDistPts	Equal distance points on edges
	*	\param[out]	stripes			Sampled stripe images
	*	\param[out]	stripes_sobel	Stripe images obtained after applying y-Sobel operator
	*	\param[out]	return			Ouput image
	*/
	Mat SampleStripesFromEdges(Mat srcImage, Vec2Pt markerEdges, Vec3Pt2f& equalDistPts, Vec3Mat& stripes, Vec3Mat& stripes_sobel);
	/**
	*	\brief	Q3-(g) In order to find the sub-pixel accurate location of the edge, fit a parabola to the stripe. 
	*	\param[in]	markerCorner	Marker edges (corners)
	*	\param[in]	equalDistPts	Equal distance points on edges
	*	\param[in]	stripes_sobel	Stripe images obtained after applying y-Sobel operator
	*	\param[out]	subPoints		Sub-pixel accurate points on edges	
	*/
	void FindAccurateEdges(Vec2Pt markerCorner, Vec3Pt2f equalDistPts, Vec3Mat stripes_sobel, Vec3Pt2f& subPoints);
	/**
	*	\brief	Q3-(h) Fit lines to the 6-pint sets on the edges of the markers.
	*	\param[in]	points			Source points	
	*	\param[out]	lines			Line parameters (Position_x,Position_y,Orientation_dx,Orientation_dy)
	*	\param[out]	return			Ouput image
	*/
	Mat FitLines(Vec3Pt2f points, Vec2Vec4f& lines);
	/**
	*	\brief	Q3-(i) Find accurate corners for the marekr.
	*	\param[in]	lines			Source lines (Position_x,Position_y,Orientation_dx,Orientation_dy)
	*	\param[out]	corners			Intersection points
	*	\param[out]	return			Ouput image
	*/
	Mat FindAccurateCorners(Vec2Vec4f lines, Vec2Pt& corners);
	/**
	*	\brief	Q3-(j) Draw the final lines and points on the image.
	*	\param[in]	srcImage		Source image
	*	\param[in]	lines			Marker edges
	*	\param[in]	corners			Marker corners 
	*	\param[out]	return			Ouput image
	*/
	Mat ShowFinalLinesAndPoints(Mat srcImage, Vec2Vec4f lines, Vec2Pt corners);
	/**
	*	\brief	Q3-(j) Draw the locations of the stripes.
	*	\param[in]	srcImage		Source image
	*	\param[in]	markerCorners	Marker corners
	*	\param[in]	equalDistPts	Equal distance points on edges
	*	\param[in]	stripes_sobel	Stripe images obtained after applying y-Sobel operator
	*	\param[out]	return			Ouput image
	*/
	Mat ShowStripesLocations(Mat srcImage, Vec2Pt markerCorners, Vec3Pt2f equalDistPts, Vec3Mat stripes_sobel);
};


