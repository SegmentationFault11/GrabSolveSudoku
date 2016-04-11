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

using namespace cv;
using namespace xphoto;

int main(int argc, const char * argv[]) {
    const double min_area = 550000.0;
    
    //Load image
    Mat sudoku = imread("/Users/stevenma/Desktop/sudokupuzzles/sudoku1.png", 1);
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
    vector<int> board;
    board.resize(81);
    match_num(nums, board, cell_list, cell_size, board_im);
    //imshow("nums", nums);
    //imshow("cells", cells); cvWaitKey();
    
    
    Mat curr;
    int num = 82;
    for (int i = 0; i < 81; ++i) {
        if (board_im[i].data)  {
            string name = "/Users/stevenma/Desktop/sdk/sdk.font1.exp" + to_string(num++) + ".tif";
            //board_im[i].convertTo(curr, CV_8U);
            resize(board_im[i], board_im[i], Size(5*board_im[i].size().width,5*board_im[i].size().height), INTER_NEAREST);
            //bitwise_not(board_im[i], board_im[i]);
            adaptiveThreshold(board_im[i], board_im[i], <#double maxValue#>, <#int adaptiveMethod#>, <#int thresholdType#>, <#int blockSize#>, <#double C#>)
            //threshold(board_im[i], board_im[i], 0, 255, THRESH_BINARY);
            imshow(to_string(i), board_im[i]);
//            if (imwrite(name, board_im[i])) {
//                cout << name << " write success" << endl;
//            }
//            else {
//                cout << name << " write fail" << endl;
//            }
        }
    }
    
    
//    imshow("part", board_im[3]);
    
//    imshow("nums", nums);
//    imshow("cells:", cells);
//    
    int grid[9][9] = {{0}};
    tesseract::TessBaseAPI *myOCR = new tesseract::TessBaseAPI();
    
    myOCR->Init(NULL, "sdk");
    
    myOCR->SetVariable("tessedit_char_whitelist","123456789");
    myOCR->SetPageSegMode(tesseract::PSM_SINGLE_CHAR);
    
    for (int i = 0; i < 81; ++i) {
        if (board_im[i].data)  {
            
            //imshow(to_string(i), board_im[i]);
            myOCR->SetImage((uchar*)board_im[i].data, board_im[i].size().width, board_im[i].size().height, board_im[i].channels(), (int)board_im[i].step1());
            
            grid[i/9][i%9] = atoi(myOCR->GetUTF8Text());
        }
    }
    
    printGrid(grid);
    cout << "\n\n\n" << endl;
    if (solve(grid))
        printGrid(grid);
    else
        cout << "No!" << endl;
    
    cvWaitKey();
    return 0;
}
