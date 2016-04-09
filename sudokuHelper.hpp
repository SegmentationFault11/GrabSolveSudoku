#ifndef sudokuHelper_hpp
#define sudokuHelper_hpp

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

Point paintBorder(Mat inputMat);

void paintBlack(Mat inputMat, int color);

Mat adjSize (Mat &input, int std_sz);


#endif /* sudokuHelper_hpp */
