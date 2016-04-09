//
//  findCell.cpp
//  testCV2
//
//  Created by Trojan Horse on 5/04/2016.
//  Copyright © 2016 NotYourBusiness. All rights reserved.
//

#include "numCell.hpp"


void findCell(Mat &sudoku, Mat &cells, Mat &nums) {
    Mat sudoku_blur;
    GaussianBlur(sudoku, sudoku_blur, Size(7,7), 0);
    
    adaptiveThreshold(sudoku_blur, nums, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 5, 2);
    adaptiveThreshold(sudoku, cells, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 15, 2);
    
    Mat kernel1 = (Mat_<uchar>(3,3) << 0,1,0,1,1,1,0,1,0);
    Mat kernel2 = (Mat_<uchar>(5,5) << 0,0,1,0,0, 0,1,1,1,0, 1,1,1,1,1, 0,1,1,1,0, 0,0,1,0,0);
    Point maxPt = paintBorder(cells);
    imshow("temp", nums);
    
    floodFill(cells, Point(0,0), CV_RGB(255,255,255));
    floodFill(cells, maxPt, CV_RGB(255,255,255));
    floodFill(nums, maxPt, CV_RGB(255,255,255));
    
    paintBlack(cells, 64);
    paintBlack(nums, 64);
    
    dilate(cells, cells, kernel2);
    
    GaussianBlur(cells, cells, Size(5,5), 0);
    threshold(cells, cells, 50, 255, THRESH_BINARY_INV);
    dilate(cells, cells, kernel2);
    
    bitwise_not(nums, nums);
    
    bitwise_and(nums, cells, nums);
    
    dilate(nums, nums, kernel1);
    
    GaussianBlur(nums, nums, Size(7,7), 0);
    bitwise_not(nums, nums);
    bitwise_not(cells, cells);
    threshold(nums, nums, 75, 127, THRESH_BINARY_INV);
}