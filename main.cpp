#include <opencv2/opencv.hpp>
#include <opencv2/xphoto.hpp>
#include <opencv2/xphoto/white_balance.hpp>

#include <iostream>
#include <vector>
#include <cmath>

#include "sudokuHelper.hpp"
#include "straighten.hpp"
#include "numCell.hpp"
#include "nameCell.hpp"

using namespace cv;
using namespace xphoto;

int main(int argc, const char * argv[]) {
    const double min_area = 16000.0;
    
    //Load image
    Mat sudoku = imread("/Users/stevenma/Desktop/EECS 442/testCV2/testCV2/sudoku.jpg", 1);
    cvtColor(sudoku, sudoku, CV_BGR2GRAY);
    
    sudoku = adjSize(sudoku, min_area);
    
    sudoku = straighten(sudoku);

    Mat cells, nums;
    findCell(sudoku, cells, nums);
    
    vector<Rect> cell_list;
    cell_list.resize(81);
    int cell_size = 0;
    name_cell(cells, cell_list, cell_size);
    
    
    vector<Mat> board_im;
    board_im.resize(81);
    vector<int> board;
    board.resize(81);
    match_num(nums, board, cell_list, cell_size, board_im);
    
    for (int i = 0; i < 81; ++i) {
        if (i%9 == 0) cout << endl;
        cout << board[i] << " ";
    }
    cout << endl;
    
    for (int i = 0; i < 81; ++i) {
        if (board[i]) imshow(to_string(i), board_im[i]);
    }
    
    
    //imshow("part", board_im[6]);
    
    imshow("nums", nums);
    imshow("cells:", cells);
    
    
    cvWaitKey();
    return 0;
}


















//int main(int argc, const char * argv[]) {
//    const double std_area = 160000.0;
//    
//    //Load image
//    Mat sudoku = imread("/Users/stevenma/Desktop/EECS 442/testCV2/testCV2/sudoku.jpg", CV_LOAD_IMAGE_GRAYSCALE);
//    
//    double im_area =  sudoku.size().height * sudoku.size().width;
//    
//    double sz_ratio = sqrt(std_area/im_area);
//    
//    Size new_sz(sudoku.size().height*sz_ratio, sudoku.size().width*sz_ratio);
//    
//    resize(sudoku, sudoku, new_sz);
//    
//    sudoku = straighten(sudoku);
//    Mat sudoku_blur;
//    
//    Mat grid = Mat(sudoku.size(), CV_8UC1);
//    Mat num = Mat(sudoku.size(), CV_8UC1);
//    GaussianBlur(sudoku, sudoku_blur, Size(11,11), 0);
//    
//    imshow("blur", sudoku);
//    
//    imshow("balance", sudoku);
//    //threshold(sudoku, sudoku, 230, 255, THRESH_BINARY);
//    imshow("shrp", sudoku);
//    
//    adaptiveThreshold(sudoku_blur, num, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 5, 2);
//    adaptiveThreshold(sudoku, grid, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 15, 2);
//    
//    bitwise_not(grid, grid);
//    Mat kernel1 = (Mat_<uchar>(3,3) << 0,1,0,1,1,1,0,1,0);
//    Mat kernel2 = (Mat_<uchar>(5,5) << 0,0,1,0,0, 0,1,1,1,0, 1,1,1,1,1, 0,1,1,1,0, 0,0,1,0,0);
//    //below is code for detecting the outer box, all items are colored 64 in the process
//    Point maxPt = shLargestFlood(grid);
//    
//    //color the largest area white
//    floodFill(grid, Point(0,0), CV_RGB(255,255,255));
//    floodFill(grid, maxPt, CV_RGB(255,255,255));
//    floodFill(num, maxPt, CV_RGB(255,255,255));
//    //this loop goes over the image and colors all the grey areas black
//    shGetRidOfColor(grid, 64);
//    shGetRidOfColor(num, 64);
//    
//    dilate(grid, grid, kernel2);
//    
//    GaussianBlur(grid, grid, Size(7,7), 0);
//    threshold(grid, grid, 50, 255, THRESH_BINARY);
//    
//    bitwise_or(num, grid, num);
//    
//    bitwise_not(num, num);
//    dilate(num, num, kernel1);
//    
//    GaussianBlur(num, num, Size(5,5), 0);
//    bitwise_not(num, num);
//    adaptiveThreshold(num, num, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 5, 2);
//    
//    
//    
//    imshow("num3", num);
//    imshow("outerBox3:",grid);
//    cvWaitKey();
//    return 0;
//}

