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
    
    imshow("straight", sudoku);

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
