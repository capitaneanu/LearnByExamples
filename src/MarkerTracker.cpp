#include "MarkerTracker.h"

MarkerTracker::MarkerTracker()
{
}

MarkerTracker::~MarkerTracker()
{
}

Mat MarkerTracker::Threshold(Mat srcImage, double thValue, Mat& grayImage)
{
	srcImage.copyTo(m_originalImage);
	m_imageSize = srcImage.size();
	cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);
	threshold(grayImage, m_thresholdOutput, thValue, 255, THRESH_BINARY);
	return m_thresholdOutput;
}

Mat MarkerTracker::FindContours(Mat source, Vec2Pt& contours)
{	
	vector<vector<Point>> contours0;
	vector<Vec4i> hierarchy;
	Mat contoursImage;

	source.copyTo(contoursImage);
	findContours(contoursImage, contours0, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	contours.resize(contours0.size());

	for (size_t i = 0; i < contours0.size(); i++)
		approxPolyDP(Mat(contours0[i]), contours[i], 3, true);
		
	m_contourOutput = Mat::zeros(m_imageSize, CV_8UC3);
	drawContours(m_contourOutput, contours, -1, Scalar(0, 255, 0), 1, LINE_AA);
	contours0.clear();
	return m_contourOutput;
}

Mat MarkerTracker::ExtractMarkerContours(Vec2Pt contours, int minSize, int maxSize, int numCorners, Vec2Pt& markerContours)
{	
	m_markerContourOutput1 = cv::Mat::zeros(m_imageSize, CV_8UC3);
	Rect rect;
	for (size_t i = 0; i < contours.size(); i++) {		
		rect = boundingRect(contours[i]);
		if (rect.width > minSize &&		// Check the minimum width of polygon
			rect.height > minSize &&	// Check the minimum height of polygon
			rect.width < maxSize &&		// Check the maximum width of polygon
			rect.height < maxSize &&	// Check the maximum height of polygon
			contours[i].size() == numCorners) {	// Check the number of corners (find squares)
			markerContours.push_back(contours[i]);
		}
	}
	drawContours(m_markerContourOutput1, markerContours, -1, cv::Scalar(0, 255, 0), 1, cv::LINE_AA);
	return m_markerContourOutput1;
}

Mat MarkerTracker::DrawMarkerContours(Vec2Pt contours, Scalar edgeColor, Scalar cornerColor)
{	
	// Draw lines and circles for edges and corners of marekrs	
	m_markerContourOutput2 = cv::Mat::zeros(m_imageSize, CV_8UC3);
	for (size_t i = 0; i < contours.size(); i++) {
		for (size_t j = 0; j < contours[i].size(); j++) {
			line(m_markerContourOutput2, contours[i][j], contours[i][(j + 1) % 4], edgeColor, 1);
			circle(m_markerContourOutput2, contours[i][j], 2, cornerColor, 2);			
		}
	}	
	return m_markerContourOutput2;
}

double MarkerTracker::SampleEqualDistPoints(Point edgeEndpoint1, Point edgeEndpoint2, int numPoints, Vec1Pt2f& outPutPoints, Point2f& directionVec)
{
	outPutPoints.clear();
	// Compute equal distance h and direction vector
	directionVec = edgeEndpoint2 - edgeEndpoint1;	// Direction vector
	double h;
	h = sqrt(directionVec.dot(directionVec));			// Distance bitween two points
	directionVec /= h;									// Normalization
	h /= (numPoints+1);									// Equal distance h
	
	Point2f startPt = edgeEndpoint1;
	// Compute the equal distance points and put into the output container
	for (int i = 0; i < 6; i++) {		
		Point2f point = startPt + (i+1)*h*directionVec;
		outPutPoints.push_back(point);
	}
	return h;
}

void MarkerTracker::SampleStrips(Mat srcImg, Vec1Pt2f cenPts, Point2f direction, int width, int length, Vec1Mat& strips, Vec1Mat& strips_sobel)
{
	Mat grayImg;
	cvtColor(srcImg, grayImg, COLOR_BGR2GRAY);
	
	Point2f origin;
	Point2f samplePos;
	Point2f normal;

	normal.x = direction.y;
	normal.y = -direction.x;

	strips.clear();
	strips_sobel.clear();

	Mat sobel, sobel_abs;

	for (int k = 0; k < (int)cenPts.size(); k++) {
		origin = cenPts[k] + length / 2 * direction - (width-1) / 2 * normal;
		Mat strip(length, width, CV_8UC1);
		for (int i = 0; i < length; i++) {
			for (int j = 0; j < width; j++) {
				samplePos = origin - i*direction + j*normal;				
				strip.data[i*strip.step + j] = SampleSubPix(grayImg, samplePos);
			}
		}
		// Apply y-Sobel operator
		Sobel(strip, sobel, CV_16S, 0, 1, 3, 1, 0, BORDER_DEFAULT);
		// Convert results to CV_8U
		convertScaleAbs(sobel, sobel_abs);
		
		strips.push_back(strip);
		strips_sobel.push_back(sobel_abs);
	}
}

Mat MarkerTracker::SampleStripesFromEdges(Mat srcImage, Vec2Pt markerEdges, Vec3Pt2f& equalDistPts,	Vec3Mat& stripes, Vec3Mat& stripes_sobel)
{
	//srcImage.copyTo(m_sampledStripes);
	m_sampledStripes = cv::Mat::zeros(m_imageSize, CV_8UC3);
	// Equaldistance
	double h;
	// Edge direction vecotr & Stripe direction vector
	Point2f edgeDir, stripDir;
	
	for (int i = 0; i < (int)markerEdges.size(); i++) {
		vector<vector<Point2f>> v1;
		equalDistPts.push_back(v1);
		vector<vector<Mat>> i1;
		stripes.push_back(i1);
		stripes_sobel.push_back(i1);
		for (int j = 0; j < (int)markerEdges[i].size(); j++) {
			vector<Point2f> v2;
			equalDistPts[i].push_back(v2);
			vector<Mat> i2;
			stripes[i].push_back(i2);
			stripes_sobel[i].push_back(i2);

			// Sample 6 equal distance points from each edge.
			h = SampleEqualDistPoints(markerEdges[i][j], markerEdges[i][(j + 1) % 4], 6, equalDistPts[i][j],edgeDir);

			// Compute the stripe direction vector, which is perpendicular to the edge direction vector
			stripDir.x = edgeDir.y;
			stripDir.y = -edgeDir.x;

			// Sample stripes from edges and apply y-Sobel operator
			SampleStrips(srcImage, equalDistPts[i][j], stripDir, 3, (int)round(h), stripes[i][j], stripes_sobel[i][j]);

			//Draw 6 equal distance points
			for (int k = 0; k < (int)equalDistPts[i][j].size(); k++) {
				Point pt = equalDistPts[i][j][k];
				circle(m_sampledStripes, pt, 1, Scalar(0, 0, 255), 2);
			}
		}
	}

	return m_sampledStripes;
}

void MarkerTracker::FindStripesMaximum(Mat srcImg, Vec1Pt& result, vector<unsigned int>& values)
{
	result.clear();
	values.clear();

	unsigned int maxVal = 0;
	Point maxPixel;

	for (int i = 0; i < srcImg.rows; i++) {
		for (int j = 0; j < srcImg.cols; j++) {
			if (srcImg.data[i*srcImg.step + j] > maxVal) {
				maxVal = srcImg.data[i*srcImg.step + j];
				maxPixel.x = j;
				maxPixel.y = i;
			}
		}
	}
	values.push_back(maxVal);
	result.push_back(maxPixel);
	if (maxPixel.y > 0) {
		result.push_back(Point(maxPixel.x, maxPixel.y - 1));
		values.push_back(srcImg.data[(maxPixel.y - 1)*srcImg.step + maxPixel.x]);
	}
	if (maxPixel.y < srcImg.rows - 1) {
		result.push_back(Point(maxPixel.x, maxPixel.y + 1));
		values.push_back(srcImg.data[(maxPixel.y + 1)*srcImg.step + maxPixel.x]);
	}
}

void MarkerTracker::FitParabola(vector<unsigned int> values, double& offset)
{
	if (values.size() == 0) {
		offset = 0.0;
		return;
	}
	Eigen::MatrixXf A = Eigen::MatrixXf::Zero(3, 3);
	Eigen::VectorXf b = Eigen::VectorXf::Zero(3);

	A << 0, 0, 1,
		1, -1, 1,
		1, 1, 1;
	b << values[0], values[1], values[2];

	//Solve linear least squares sytem
	Eigen::VectorXf x = A.inverse() * b;
	/*
	*	Solving linear least squares systems - Eigen numerical library
	*	http://eigen.tuxfamily.org/dox-devel/group__LeastSquares.html
	*/
	//Solve linear least squares sytem using SVD decomposition
	//Eigen::VectorXf x = A.jacobiSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(b); 	
	//Solve linear least squares sytem using QR decomposition
	//VectorXf x = A.colPivHouseholderQr().solve(b);
	//Solve linear least squares sytem using normal equations
	//Eigen::VectorXf x = (A.transpose() * A).ldlt().solve(A.transpose() * b);

	offset = -x[1] / (2.0 * x[0]);
}

void MarkerTracker::ComputeSubAccuratePoint(Point2f edgeDirection, Point2f cenStripe, Point maxPixel, double offset, int stripeLength, Point2f& subPoint)
{	
	Point2f stripeDirection;
	stripeDirection.x = edgeDirection.y;
	stripeDirection.y = -edgeDirection.x;
	
	subPoint = cenStripe + edgeDirection * (1 - maxPixel.x) + stripeDirection * (stripeLength / 2 - maxPixel.y - offset);
}

void MarkerTracker::FindAccurateEdges(Vec2Pt markerCorner, Vec3Pt2f equalDistPts, Vec3Mat stripes_sobel, Vec3Pt2f& subPoints)
{
	Vec1Pt maxPixels;
	vector<unsigned int> maxValues;
	double offset;
	Point2f edgeDirection;

	for (int i = 0; i < (int)stripes_sobel.size(); i++) {
		Vec2Pt2f sub1;
		subPoints.push_back(sub1);
		for (int j = 0; j < (int)stripes_sobel[i].size(); j++) {
			Vec1Pt2f sub2;
			subPoints[i].push_back(sub2);
			edgeDirection = markerCorner[i][(j + 1) % 4] - markerCorner[i][j];
			edgeDirection /= sqrt(edgeDirection.dot(edgeDirection));
			for (int k = 0; k < (int)stripes_sobel[i][j].size(); k++) {
				FindStripesMaximum(stripes_sobel[i][j][k], maxPixels, maxValues);
				FitParabola(maxValues, offset);				
				Point2f subPoint;
				ComputeSubAccuratePoint(edgeDirection, equalDistPts[i][j][k], maxPixels[0], offset, stripes_sobel[i][j][k].rows, subPoint);
				subPoints[i][j].push_back(subPoint);				
			}
		}		
	}
	
	maxPixels.clear();
	maxValues.clear();
}

Mat MarkerTracker::FitLines(Vec3Pt2f points, Vec2Vec4f& lines)
{	
	//m_originalImage.copyTo(m_lineOutput);
	m_lineOutput = cv::Mat::zeros(m_imageSize, CV_8UC3);	
	RNG rng(12345);
	double lengthLine = 400;
	for (size_t i = 0; i < (int)points.size(); i++) {
		vector<Vec4f> l1;
		lines.push_back(l1);
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		for (size_t j = 0; j < (int)points[i].size(); j++) {		
			Vec4f l2;
			lines[i].push_back(l2);
			fitLine(points[i][j], lines[i][j], DIST_L2, 0, 0.01, 0.01);		
			line(m_lineOutput, Point((int)round(lines[i][j][2] - lengthLine / 2 * lines[i][j][0]), (int)round(lines[i][j][3] - lengthLine / 2 * lines[i][j][1])),
				Point((int)round(lines[i][j][2] + lengthLine / 2 * lines[i][j][0]), (int)round(lines[i][j][3] + lengthLine / 2 * lines[i][j][1])), color, 1);
		}
	}	
	return m_lineOutput;	
}

Mat MarkerTracker::FindAccurateCorners(Vec2Vec4f lines, Vec2Pt& corners)
{
	m_cornerOutput = cv::Mat::zeros(m_imageSize, CV_8UC3);
	RNG rng(12345);	
	Point2f p1, p2, d1, d2;	
	double s;
	for (size_t i = 0; i < lines.size(); i++) {
		vector<Point> v1;
		corners.push_back(v1);
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		for (size_t j = 0; j < lines[i].size(); j++) {
			Point v2;
			corners[i].push_back(v2);
			p1.x = lines[i][j][2];
			p1.y = lines[i][j][3];
			d1.x = lines[i][j][0];
			d1.y = lines[i][j][1];
			p2.x = lines[i][(j + 1) % 4][2];
			p2.y = lines[i][(j + 1) % 4][3];
			d2.x = lines[i][(j + 1) % 4][0];
			d2.y = lines[i][(j + 1) % 4][1];

			s = (d2.x / (d1.y*d2.x - d2.y*d1.x))*(p2.y - p1.y + (d2.y / d2.x*(p1.x - p2.x)));

			corners[i][j] = (p1 + s * d1);			
			circle(m_cornerOutput, corners[i][j], 2, color, 2);
		}
	}
	return m_cornerOutput;
}

Mat MarkerTracker::ShowFinalLinesAndPoints(Mat srcImage, Vec2Vec4f lines, Vec2Pt corners)
{
	srcImage.copyTo(m_finalLinesAndPoints);
	RNG rng(12345);
	double lengthLine = 400;
	for (size_t i = 0; i < lines.size(); i++) {
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		for (size_t j = 0; j < lines[i].size(); j++) {
			line(m_finalLinesAndPoints, Point((int)round(lines[i][j][2] - lengthLine / 2 * lines[i][j][0]), (int)round(lines[i][j][3] - lengthLine / 2 * lines[i][j][1])),
						Point((int)round(lines[i][j][2] + lengthLine / 2 * lines[i][j][0]), (int)round(lines[i][j][3] + lengthLine / 2 * lines[i][j][1])), color, 1);
			circle(m_finalLinesAndPoints, corners[i][j], 2, Scalar(0,0,255), 2);
		}
	}
	return m_finalLinesAndPoints;
}

int MarkerTracker::SampleSubPix(const Mat pSrc, const Point2f p)
{
	int x = int(floorf(p.x));
	int y = int(floorf(p.y));

	if (x < 0 || x >= pSrc.cols - 1 ||
		y < 0 || y >= pSrc.rows - 1)
		return 127;

	int dx = int(256 * (p.x - floorf(p.x)));
	int dy = int(256 * (p.y - floorf(p.y)));

	unsigned char* i = (unsigned char*)((pSrc.data + y * pSrc.step) + x);
	int a = i[0] + ((dx * (i[1] - i[0])) >> 8);
	i += pSrc.step;
	int b = i[0] + ((dx * (i[1] - i[0])) >> 8);
	return a + ((dy * (b - a)) >> 8);
}

Mat MarkerTracker::ShowStripesLocations(Mat srcImage, Vec2Pt markerCorners, Vec3Pt2f equalDistPts, Vec3Mat stripes_sobel)
{
	srcImage.copyTo(m_stripesLocations);
	RNG rng(12345);
	Point2f edgeDirection, stripeDirection;
	Point p1, p2, p3, p4;

	for (size_t i = 0; i < equalDistPts.size(); i++) {		
		for (size_t j = 0; j < equalDistPts[i].size(); j++) {
			edgeDirection = markerCorners[i][(j + 1) % 4] - markerCorners[i][j];
			edgeDirection /= sqrt(edgeDirection.dot(edgeDirection));
			stripeDirection.x = edgeDirection.y;
			stripeDirection.y = -edgeDirection.x;
			Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			for (size_t k = 0; k < equalDistPts[i][j].size(); k++) {
				circle(m_stripesLocations, equalDistPts[i][j][k], 1, Scalar(255, 0, 0), 2);
				p1 = equalDistPts[i][j][k] + stripeDirection * stripes_sobel[i][j][k].rows / 2 + edgeDirection * 1.5;
				p2 = equalDistPts[i][j][k] - stripeDirection * stripes_sobel[i][j][k].rows / 2 + edgeDirection * 1.5;
				p3 = equalDistPts[i][j][k] - stripeDirection * stripes_sobel[i][j][k].rows / 2 - edgeDirection * 1.5;
				p4 = equalDistPts[i][j][k] + stripeDirection * stripes_sobel[i][j][k].rows / 2 - edgeDirection * 1.5;
				line(m_stripesLocations, p1, p2, color, 1);
				line(m_stripesLocations, p2, p3, color, 1);
				line(m_stripesLocations, p3, p4, color, 1);
				line(m_stripesLocations, p4, p1, color, 1);
			}			
		}
	}
	return m_stripesLocations;
}