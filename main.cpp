#include <opencv2/opencv.hpp>
#include <opencv2/xphoto.hpp>
#include <opencv2/xphoto/white_balance.hpp>

#include <tesseract/baseapi.h>
#include <tesseract/strngs.h>

#include <iostream>
#include <vector>
#include <cmath>
#include <string>

#include "helpers.hpp"
#include "sudokuSolver.hpp"

using namespace std;
using namespace cv;
using namespace xphoto;
using namespace tesseract;

int main(int argc, const char * argv[]) {
    const double min_area = 1500000.0;
    
    //Load image
    Mat sudoku = imread("/Users/stevenma/Desktop/sudokupuzzles/sudoku2.png", 1);
    if (!sudoku.data) exit(1);
    
    balanceWhite(sudoku, sudoku, WHITE_BALANCE_SIMPLE);
    cvtColor(sudoku, sudoku, CV_BGR2GRAY);

    sudoku = straighten(sudoku);
    
    adjSize(sudoku, sudoku, min_area);

    Mat cells, nums;
    findCell(sudoku, cells, nums);
    
    vector<Rect> cell_list;
    cell_list.resize(81);
    int cell_size = 0;
    name_cell(cells, cell_list, cell_size);    
    
    vector<Mat> board_im;
    board_im.resize(81);
    match_num(nums, cell_list, cell_size, board_im);
    
    Mat curr;
    int num = 0;
    for (int i = 0; i < 81; ++i) {
        if (board_im[i].data)  {
            string name = "/Users/stevenma/Desktop/sdk/sdk.font1.exp" + to_string(num++) + ".tif";
            board_im[i].convertTo(curr, CV_8U);
            //resize(board_im[i], board_im[i], Size(5*board_im[i].size().width,5*board_im[i].size().height), INTER_NEAREST);
            //bitwise_not(board_im[i], board_im[i]);
            //bitwise_not(board_im[i], board_im[i]);
            //adaptiveThreshold(board_im[i], board_im[i], 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 5, 2);
            //threshold(board_im[i], board_im[i], 0, 255, THRESH_BINARY);
            imshow(to_string(i), curr);
            
            cout << "x = " << curr.size().width << " y = " << curr.size().height << endl;
            if (imwrite(name, curr)) {
                cout << name << " write success" << endl;
            }
            else {
                cout << name << " write fail" << endl;
            }
        }
    }
    
    
    int grid[9][9] = {{0}};
    TessBaseAPI *myOCR = new TessBaseAPI();
    
    myOCR->Init(NULL, "tla");
    
    myOCR->SetVariable("tessedit_char_whitelist","123456789");
    myOCR->SetPageSegMode(PSM_SINGLE_CHAR);
    
    for (int i = 0; i < 81; ++i) {
        if (board_im[i].data)  {
            
            imshow(to_string(i), board_im[i]);
            myOCR->SetImage((uchar*)board_im[i].data, board_im[i].size().width, board_im[i].size().height, board_im[i].channels(), (int)board_im[i].step1());
            
            grid[i/9][i%9] = atoi(myOCR->GetUTF8Text());
        }
    }
    
//    printGrid(grid);
//    cout << "\n\n\n" << endl;
//    if (solve(grid))
//        printGrid(grid);
//    else
//        cout << "No!" << endl;
    
    cvWaitKey();
    return 0;
}
