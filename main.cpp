#include <opencv2/opencv.hpp>
#include <opencv2/xphoto.hpp>
#include <opencv2/xphoto/white_balance.hpp>

#include <tesseract/baseapi.h>
#include <tesseract/strngs.h>

#include <iostream>
#include <vector>
#include <cmath>

#include "helpers.hpp"

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
    
//    for (int i = 0; i < 81; ++i) {
//        if (board[i]) imshow(to_string(i), board_im[i]);
//    }
    
    
    //imshow("part", board_im[6]);
    
    imshow("nums", nums);
    imshow("cells:", cells);
  
//    int grid[N][N] =
//       {{3,0,6,5,0,8,4,0,0},
//        {5,2,0,0,0,0,0,0,0},
//        {0,8,7,0,0,0,0,3,1},
//        {0,0,3,0,1,0,0,8,0},
//        {9,0,0,8,6,3,0,0,5},
//        {0,5,0,0,9,0,6,0,0},
//        {1,3,0,0,0,0,2,5,0},
//        {0,0,0,0,0,0,0,7,4},
//        {0,0,5,2,0,6,3,0,0}};
//    
//    if (solve(grid))
//        printGrid(grid);
//    else
//        cout << "No!" << endl;
    
    
    
    cvWaitKey();
    return 0;
}
