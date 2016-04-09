//
//  nameCell.hpp
//  testCV2
//
//  Created by Trojan Horse on 5/04/2016.
//  Copyright © 2016 NotYourBusiness. All rights reserved.
//

#ifndef nameCell_hpp
#define nameCell_hpp

#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

#include <opencv2/opencv.hpp>
#include <tesseract/apitypes.h>

using namespace std;
using namespace cv;



void name_cell(Mat &cells, vector<Rect> &boxes, int &cell_size);

void match_num(Mat &nums, vector<int> &board, vector<Rect> &cell_list, int cell_size, vector<Mat> &board_im);

int dim_floodFill(Mat &im, Point pt, uchar color, Rect &dim, int max_height, int max_width);

bool compRecty(Rect i, Rect j);

bool compRectx(Rect i, Rect j);

#endif /* nameCell_hpp */
