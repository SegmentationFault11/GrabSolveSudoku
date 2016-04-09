//
//  findCell.hpp
//  testCV2
//
//  Created by Trojan Horse on 5/04/2016.
//  Copyright © 2016 NotYourBusiness. All rights reserved.
//

#ifndef findCell_hpp
#define findCell_hpp

#include <opencv2/opencv.hpp>
#include "sudokuHelper.hpp"

using namespace cv;
using namespace std;

void findCell(Mat &sudoku, Mat &cells, Mat &nums);

#endif /* findCell_hpp */
