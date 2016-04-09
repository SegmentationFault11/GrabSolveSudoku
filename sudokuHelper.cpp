//
//  sudokuHelper.cpp
//  testCV2
//
//  Created by Trojan Horse on 5/04/2016.
//  Copyright © 2016 NotYourBusiness. All rights reserved.
//

#include "sudokuHelper.hpp"

#include <opencv/cv.h>

using namespace cv;


Point paintBorder(Mat inputMat){
    int max = 0;
    Point maxPt;
    
    for(int y = 0;y < inputMat.size().height; ++y)
    {
        uchar *row = inputMat.ptr(y);
        for(int x = 0;x < inputMat.size().width; x++)
        {
            if(row[x]>=128)
            {
                int area = floodFill(inputMat, Point(x,y), CV_RGB(0,0,64));
                if(area>max)
                {
                    maxPt = Point(x,y);
                    max = area;
                }
            }
        }
    }
    return maxPt;
}

void paintBlack(Mat inputMat, int color){
    for(int y = 0;y < inputMat.size().height; y++)
    {
        uchar *row = inputMat.ptr(y);
        for(int x = 0;x < inputMat.size().width; ++x)
        {
            if(row[x]==color)
                floodFill(inputMat, Point(x,y), CV_RGB(0,0,0));
        }
    }
}

Mat adjSize (Mat &input, int std_sz) {
    Mat rst;
    
    double im_area =  input.size().height * input.size().width;
    
    if (im_area > std_sz) return input;
    
    double sz_ratio = sqrt(std_sz/im_area);
    
    Size new_sz(input.size().height*sz_ratio, input.size().width*sz_ratio);
    
    resize(input, rst, new_sz);
    
    return rst;
}