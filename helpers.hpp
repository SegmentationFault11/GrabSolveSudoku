#ifndef helpers_hpp
#define helpers_hpp

#include <opencv2/opencv.hpp>
#include <opencv2/xphoto.hpp>
#include <opencv2/xphoto/white_balance.hpp>

#include <tesseract/baseapi.h>
#include <tesseract/strngs.h>

#include <iostream>
#include <vector>
#include <stack>
#include <cmath>

using namespace std;
using namespace cv;

Mat straighten(Mat &sudoku);

void drawLine(Vec2f line, Mat &img, Scalar rgb = CV_RGB(0,0,255));

void mergeRelatedLines(vector<Vec2f> *lines, Mat &img);

void findCell(Mat &sudoku, Mat &cells, Mat &nums);

Point paintBorder(Mat inputMat);

void paintBlack(Mat inputMat, int color);

Mat adjSize (Mat &input, int std_sz);

void name_cell(Mat &cells, vector<Rect> &boxes, int &cell_size);

void match_num(Mat &nums, vector<int> &board, vector<Rect> &cell_list, int cell_size, vector<Mat> &board_im);

int dim_floodFill(Mat &im, Point pt, uchar color, Rect &dim, int max_height, int max_width);

bool compRecty(Rect i, Rect j);

bool compRectx(Rect i, Rect j);


#endif /* helpers_hpp */
