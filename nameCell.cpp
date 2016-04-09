#include "nameCell.hpp"

void name_cell(Mat &cells, vector<Rect> &cell_list, int &cell_size) {
    unsigned height = cells.size().height;
    unsigned width = cells.size().width;
    unsigned upper_area = (height*width*0.9)/81;
    unsigned lower_area = (height*width*0.55)/81;
    
    unsigned num_found = 0;
    unsigned curr_area = 0;
    double curr_ratio = 0;
    
    cell_size = 0;
    
    Rect curr_dim;
    
    for (int i = 0; i < height; ++i) {
        uchar *p = cells.ptr<uchar>(i);
        for (int j = 0; j < width; ++j) {
            if (static_cast<int>(p[j]) == 0) {
                
                curr_area = dim_floodFill(cells, Point(j,i), 127, curr_dim, height, width);
                curr_ratio = (double)curr_dim.height/(double)curr_dim.width;
                
                if (curr_area < upper_area && curr_area > lower_area && curr_ratio < 1.3 && curr_ratio > 0.75) {
                    cell_list[num_found++] = curr_dim;
                    cell_size += curr_area;
                    if (num_found == 81) break;
                }
                else {
                    floodFill(cells, Point(j,i), 255);
                }
            }
        }
        if (num_found == 81) break;
    }
    
    //cout << "cell_size_l = " << cell_size << endl;
    
    cell_size /= num_found;
    
    //cout << "cell_size_s = " << cell_size << endl;
    
    sort(cell_list.begin(), cell_list.end(), compRecty);
    
    for (int i = 0; i < 81;i = i + 9)
        sort(cell_list.begin() + i, cell_list.begin() + i + 9, compRectx);
}

void match_num(Mat &nums, vector<int> &board, vector<Rect> &cell_list, int cell_size, vector<Mat> &board_im) {
    for (int i = 0; i < 81; ++i) {
        board[i] = 0;
    }
    
    unsigned height = nums.size().height;
    unsigned width = nums.size().width;
    unsigned upper_area = (cell_size*0.6);
    unsigned lower_area = (cell_size*0.1);
    
    //cout << "what = " << (cell_size*0.7) << endl;
    
    unsigned num_found = 0;
    unsigned curr_area = 0;
    double curr_ratio = 0;
    bool found = false;
    
    for (int i = 0; i < 81; ++i) {
        circle(nums, Point(cell_list[i].x, cell_list[i].y), 3, 0, -1);
        circle(nums, Point(cell_list[i].x + cell_list[i].width, cell_list[i].y), 3, 0, -1);
        circle(nums, Point(cell_list[i].x, cell_list[i].y + cell_list[i].height), 3, 0, -1);
        circle(nums, Point(cell_list[i].x + cell_list[i].width, cell_list[i].y + cell_list[i].height), 3, 0, -1);
    }
    
    Rect curr_dim;
    Point curr_center;
    for (int i = 0; i < height; ++i) {
        uchar *p = nums.ptr<uchar>(i);
        for (int j = 0; j < width; ++j) {
            if (static_cast<int>(p[j]) == 127) {
                
                dim_floodFill(nums, Point(j,i), 255, curr_dim, height, width);
                
                curr_area = curr_dim.height*curr_dim.width;
                curr_ratio = (double)curr_dim.height/(double)curr_dim.width;
                
                //cout << "area = " << curr_area << " up = " << upper_area << " la = " << lower_area << " ratio = " << curr_ratio << endl;
                
                if (curr_area < upper_area && curr_area > lower_area && curr_ratio < 3 && curr_ratio > 1.1) {
                    curr_center.x = curr_dim.x + curr_dim.width/2;
                    curr_center.y = curr_dim.y + curr_dim.height/2;
                    
                    found = false;
                    for (int k = 0; !found && k < 81; ++k) {
                        Rect curr_cell = cell_list[k];
                        if ((curr_center.x < curr_cell.x + curr_cell.width) &&
                            (curr_center.x > curr_cell.x) &&
                            (curr_center.y < curr_cell.y + curr_cell.height) &&
                            (curr_center.y > curr_cell.y)) {
                            board[k] = 1;
                            board_im[k] = nums(curr_dim);
                            found = true;
                        }
                    }
                    if (!found) floodFill(nums, Point(j,i), 0);
                    if (num_found == 81) break;
                }
                else floodFill(nums, Point(j,i), 0);
            }
        }
    }
}

int dim_floodFill(Mat &im, Point pt, uchar color, Rect &dim, int max_height, int max_width) {
    stack<Point> loc;
    int area = 1;
    
    unsigned top = pt.y;
    unsigned bot = pt.y;
    unsigned left = pt.x;
    unsigned right = pt.x;
    
    uchar ori_clr = im.at<uchar>(pt);
    im.at<uchar>(pt) = color;
    loc.push(pt);
    
    uchar *pixel = &im.at<uchar>(pt);
    Point curr_point;
    
    int xval;
    int yval;
    while(!loc.empty()) {
        curr_point = loc.top();
        loc.pop();
        if (pt.x > 0) {
            xval = curr_point.x-1;
            yval = curr_point.y;
            pixel = &im.at<uchar>(Point(xval, yval));
            if (*pixel == ori_clr) {
                *pixel = color;
                loc.push(Point(xval, yval));
                ++area;
                if (xval < left) left = xval;
            }
        }
        if (pt.x < max_width) {
            xval = curr_point.x+1;
            yval = curr_point.y;
            pixel = &im.at<uchar>(Point(xval, yval));
            if (*pixel == ori_clr) {
                *pixel = color;
                loc.push(Point(xval, yval));
                ++area;
                if (xval > right) right = xval;
            }
        }
        if (pt.y > 0) {
            xval = curr_point.x;
            yval = curr_point.y-1;
            pixel = &im.at<uchar>(Point(xval, yval));
            if (*pixel == ori_clr) {
                *pixel = color;
                loc.push(Point(xval, yval));
                ++area;
                if (yval < top) top = yval;
            }
        }
        if (pt.y < max_height) {
            xval = curr_point.x;
            yval = curr_point.y+1;
            pixel = &im.at<uchar>(Point(xval, yval));
            if (*pixel == ori_clr) {
                *pixel = color;
                loc.push(Point(xval, yval));
                ++area;
                if (yval > bot) bot = yval;
            }
        }
    }
    
    if (left == 0) cout << "what" << endl;
    dim.x = left;
    dim.y = top;
    dim.height = bot - top;
    dim.width = right - left;
    
    return area;
}

bool compRecty(Rect i, Rect j) {return i.y < j.y;}
    
bool compRectx(Rect i, Rect j) {return i.x < j.x;}