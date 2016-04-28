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
    
    // Load image. Be sure to use your image directory.
    Mat sudoku = imread("/Users/stevenma/Desktop/SampleImages/sudoku11.png", 1);
    
    // Make sure an image is loaded.
    if (!sudoku.data) {
        cout << "nothing loaded" << endl;
        exit(1);
    }
    
    // Do white balancing
    balanceWhite(sudoku, sudoku, WHITE_BALANCE_SIMPLE);
    cvtColor(sudoku, sudoku, CV_BGR2GRAY);

    // Do edge warping
    sudoku = straighten(sudoku);
    
    // Resize smaller images to larger ones
    adjSize(sudoku, sudoku, min_area);

    // Detect the positions cells and digits
    Mat cells, nums;
    findCell(sudoku, cells, nums);
    
    // Find the position and dimension of each cell and match the cells to their correct position
    vector<Rect> cell_list;
    cell_list.resize(81);
    int cell_size = 0;
    name_cell(cells, cell_list, cell_size);
    
    // Match each potiential digit to their correct cell
    vector<Mat> board_im;
    board_im.resize(81);
    match_num(nums, cell_list, cell_size, board_im);
    
    // Initializing the Tesseract API
    int grid[9][9] = {{0}};
    TessBaseAPI *myOCR = new TessBaseAPI();
    myOCR->Init(NULL, "sdk");
    myOCR->SetVariable("tessedit_char_whitelist","123456789");
    myOCR->SetPageSegMode(PSM_SINGLE_CHAR);

    // Recognise the digits using Tesseract
    for (int i = 0; i < 81; ++i) {
        if (board_im[i].data)  {
            myOCR->SetImage((uchar*)board_im[i].data, board_im[i].size().width, board_im[i].size().height, board_im[i].channels(), (int)board_im[i].step1());
            grid[i/9][i%9] = atoi(myOCR->GetUTF8Text());
        }
    }
    
    // Print out the board, empty squares are represented by 0
    printGrid(grid);
    cout << "\n\n\n" << endl;
    
    // Solve the puzzle and print it out. If the puzzle is unsolvable or if the image detection went wrong, we print "No!"
    if (solve(grid))
        printGrid(grid);
    else
        cout << "No!" << endl;
    
    return 0;
}




// This is the code that saves the .tif files of the images to a folder for Tesseract training.
//
//
//    Mat curr;
//    int num = 0;
//    for (int i = 0; i < 81; ++i) {
//        if (board_im[i].data)  {
//            string name = "/Users/stevenma/Desktop/sdk/sdk.font4.exp" + to_string(num++) + ".tif";
//            imshow(to_string(i), curr);
//
//            cout << "x = " << curr.size().width << " y = " << curr.size().height << endl;
//            if (imwrite(name, curr)) {
//                cout << name << " write success" << endl;
//            }
//            else {
//                cout << name << " write fail" << endl;
//            }
//        }
//    }
