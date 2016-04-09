#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#ifndef straighten_hpp
#define straighten_hpp

using namespace std;
using namespace cv;

Mat straighten(Mat &sudoku);

void drawLine(Vec2f line, Mat &img, Scalar rgb = CV_RGB(0,0,255));

void mergeRelatedLines(vector<Vec2f> *lines, Mat &img);


#endif /* straighten_hpp */
