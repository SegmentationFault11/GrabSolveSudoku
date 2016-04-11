#include "helpers.hpp"

Mat straighten(Mat &sudoku) {
    Mat original = sudoku.clone();
    
    Mat outerBox = Mat(sudoku.size(), CV_8UC1);
    
    GaussianBlur(sudoku, sudoku, Size(11,11), 0);
    
    adaptiveThreshold(sudoku, outerBox, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 5, 2);
    
    bitwise_not(outerBox, outerBox);
    
    Mat kernel = (Mat_<uchar>(3,3) << 0,1,0,1,1,1,0,1,0);
    dilate(outerBox, outerBox, kernel);
    
    int max= -1;
    
    Point maxPt;
    
    for(int y = 0; y < outerBox.size().height; ++y) {
        uchar *row = outerBox.ptr(y);
        for(int x=0; x < outerBox.size().width; ++x) {
            if(row[x] >= 128) {
                
                int area = floodFill(outerBox, Point(x,y), CV_RGB(0,0,64));
                
                if(area > max) {
                    maxPt = Point(x, y);
                    max = area;
                }
            }
        }
    }
    
    floodFill(outerBox, maxPt, CV_RGB(255,255,255));
    
    for(int y = 0; y < outerBox.size().height; ++y) {
        uchar *row = outerBox.ptr(y);
        for(int x = 0; x < outerBox.size().width; ++x) {
            if(row[x]==64 && x!=maxPt.x && y!=maxPt.y) {
                floodFill(outerBox, Point(x,y), CV_RGB(0,0,0));
            }
        }
    }
    
    erode(outerBox, outerBox, kernel);
    
    vector<Vec2f> lines;
    HoughLines(outerBox, lines, 1, CV_PI/180, 200);
    
    mergeRelatedLines(&lines, sudoku); // Add this line
    
    for(int i=0;i<lines.size();i++) {
        drawLine(lines[i], outerBox, CV_RGB(0,0,128));
    }
    
    
    Vec2f topEdge = Vec2f(1000,1000);
    Vec2f bottomEdge = Vec2f(-1000,-1000);
    Vec2f leftEdge = Vec2f(1000,1000);
    double leftXIntercept=100000;
    Vec2f rightEdge = Vec2f(-1000,-1000);
    double rightXIntercept=0;
    
    for(int i=0;i<lines.size();i++) {
        
        Vec2f current = lines[i];
        
        float p=current[0];
        
        float theta=current[1];
        
        if(p==0 && theta==-100)
            continue;
        
        double xIntercept, yIntercept;
        xIntercept = p/cos(theta);
        yIntercept = p/(cos(theta)*sin(theta));
        
        if(theta>CV_PI*80/180 && theta<CV_PI*100/180) {
            if(p<topEdge[0])
                topEdge = current;
            
            if(p>bottomEdge[0])
                bottomEdge = current;
        }
        
        else if(theta<CV_PI*10/180 || theta>CV_PI*170/180) {
            if(xIntercept>rightXIntercept) {
                rightEdge = current;
                rightXIntercept = xIntercept;
            }
            else if(xIntercept<=leftXIntercept) {
                leftEdge = current;
                leftXIntercept = xIntercept;
            }
        }
    }
    
    drawLine(topEdge, sudoku, CV_RGB(0,0,0));
    drawLine(bottomEdge, sudoku, CV_RGB(0,0,0));
    drawLine(leftEdge, sudoku, CV_RGB(0,0,0));
    drawLine(rightEdge, sudoku, CV_RGB(0,0,0));
    
    Point left1, left2, right1, right2, bottom1, bottom2, top1, top2;
    
    int height=outerBox.size().height;
    
    int width=outerBox.size().width;
    
    if(leftEdge[1] != 0) {
        left1.x = 0;
        left1.y = leftEdge[0]/sin(leftEdge[1]);
        left2.x = width;
        left2.y = -left2.x/tan(leftEdge[1]) + left1.y;
    }
    else {
        left1.y = 0;
        left1.x = leftEdge[0]/cos(leftEdge[1]);
        left2.y = height;
        left2.x = left1.x - height*tan(leftEdge[1]);
    }
    
    if(rightEdge[1]!=0) {
        right1.x = 0;
        right1.y = rightEdge[0]/sin(rightEdge[1]);
        right2.x = width;
        right2.y = -right2.x/tan(rightEdge[1]) + right1.y;
    }
    else {
        right1.y = 0;
        right1.x = rightEdge[0]/cos(rightEdge[1]);
        right2.y = height;
        right2.x = right1.x - height*tan(rightEdge[1]);
        
    }
    
    bottom1.x = 0;
    bottom1.y = bottomEdge[0]/sin(bottomEdge[1]);
    
    bottom2.x = width;
    bottom2.y = -bottom2.x/tan(bottomEdge[1]) + bottom1.y;
    
    top1.x = 0;
    top1.y = topEdge[0]/sin(topEdge[1]);
    top2.x = width;
    top2.y = -top2.x/tan(topEdge[1]) + top1.y;
    
    double leftA = left2.y-left1.y;
    double leftB = left1.x-left2.x;
    double leftC = leftA*left1.x + leftB*left1.y;
    
    double rightA = right2.y-right1.y;
    double rightB = right1.x-right2.x;
    double rightC = rightA*right1.x + rightB*right1.y;
    
    double topA = top2.y-top1.y;
    double topB = top1.x-top2.x;
    double topC = topA*top1.x + topB*top1.y;
    
    double bottomA = bottom2.y-bottom1.y;
    double bottomB = bottom1.x-bottom2.x;
    double bottomC = bottomA*bottom1.x + bottomB*bottom1.y;
    
    double detTopLeft = leftA*topB - leftB*topA;
    
    CvPoint ptTopLeft = cvPoint((topB*leftC - leftB*topC)/detTopLeft, (leftA*topC - topA*leftC)/detTopLeft);
    
    double detTopRight = rightA*topB - rightB*topA;
    
    CvPoint ptTopRight = cvPoint((topB*rightC-rightB*topC)/detTopRight, (rightA*topC-topA*rightC)/detTopRight);
    
    double detBottomRight = rightA*bottomB - rightB*bottomA;
    CvPoint ptBottomRight = cvPoint((bottomB*rightC-rightB*bottomC)/detBottomRight, (rightA*bottomC-bottomA*rightC)/detBottomRight);
    double detBottomLeft = leftA*bottomB-leftB*bottomA;
    CvPoint ptBottomLeft = cvPoint((bottomB*leftC-leftB*bottomC)/detBottomLeft, (leftA*bottomC-bottomA*leftC)/detBottomLeft);
    
    int maxLength = (ptBottomLeft.x-ptBottomRight.x)*(ptBottomLeft.x-ptBottomRight.x) + (ptBottomLeft.y-ptBottomRight.y)*(ptBottomLeft.y-ptBottomRight.y);
    int temp = (ptTopRight.x-ptBottomRight.x)*(ptTopRight.x-ptBottomRight.x) + (ptTopRight.y-ptBottomRight.y)*(ptTopRight.y-ptBottomRight.y);
    
    if(temp>maxLength) maxLength = temp;
    
    temp = (ptTopRight.x-ptTopLeft.x)*(ptTopRight.x-ptTopLeft.x) + (ptTopRight.y-ptTopLeft.y)*(ptTopRight.y-ptTopLeft.y);
    
    if(temp>maxLength) maxLength = temp;
    
    temp = (ptBottomLeft.x-ptTopLeft.x)*(ptBottomLeft.x-ptTopLeft.x) + (ptBottomLeft.y-ptTopLeft.y)*(ptBottomLeft.y-ptTopLeft.y);
    
    if(temp>maxLength) maxLength = temp;
    
    maxLength = sqrt((double)maxLength);
    
    Point2f src[4], dst[4];
    
    src[0] = ptTopLeft;
    dst[0] = Point2f(10,10);
    src[1] = ptTopRight;
    dst[1] = Point2f(maxLength-10, 10);
    src[2] = ptBottomRight;
    dst[2] = Point2f(maxLength-10, maxLength-10);
    src[3] = ptBottomLeft;
    dst[3] = Point2f(10, maxLength-10);
    
    Mat undistorted = Mat(Size(maxLength, maxLength), CV_8UC1);
    cv::warpPerspective(original, undistorted, cv::getPerspectiveTransform(src, dst), Size(maxLength, maxLength));
    
    return undistorted;
}


void drawLine(Vec2f line, Mat &img, Scalar rgb) {
    if(line[1]!=0) {
        float m = -1/tan(line[1]);
        
        float c = line[0]/sin(line[1]);
        
        cv::line(img, Point(0, c), Point(img.size().width, m*img.size().width+c), rgb);
    }
    else
        cv::line(img, Point(line[0], 0), Point(line[0], img.size().height), rgb);
    
    
}


void mergeRelatedLines(vector<Vec2f> *lines, Mat &img) {
    vector<Vec2f>::iterator current;
    for(current=lines->begin();current!=lines->end();current++) {
        if((*current)[0]==0 && (*current)[1]==-100) continue;
        
        float p1 = (*current)[0];
        float theta1 = (*current)[1];
        
        Point pt1current, pt2current;
        if(theta1>CV_PI*45/180 && theta1<CV_PI*135/180) {
            pt1current.x=0;
            
            pt1current.y = p1/sin(theta1);
            
            pt2current.x=img.size().width;
            pt2current.y=-pt2current.x/tan(theta1) + p1/sin(theta1);
        }
        else {
            pt1current.y=0;
            
            pt1current.x=p1/cos(theta1);
            
            pt2current.y=img.size().height;
            pt2current.x=-pt2current.y/tan(theta1) + p1/cos(theta1);
            
        }
        
        vector<Vec2f>::iterator    pos;
        for(pos=lines->begin();pos!=lines->end();pos++) {
            if(*current==*pos) continue;
            if(fabs((*pos)[0]-(*current)[0])<20 && fabs((*pos)[1]-(*current)[1])<CV_PI*10/180) {
                float p = (*pos)[0];
                float theta = (*pos)[1];
                Point pt1, pt2;
                if((*pos)[1]>CV_PI*45/180 && (*pos)[1]<CV_PI*135/180) {
                    pt1.x=0;
                    pt1.y = p/sin(theta);
                    pt2.x=img.size().width;
                    pt2.y=-pt2.x/tan(theta) + p/sin(theta);
                }
                else {
                    pt1.y=0;
                    pt1.x=p/cos(theta);
                    pt2.y=img.size().height;
                    pt2.x=-pt2.y/tan(theta) + p/cos(theta);
                }
                if(((double)(pt1.x-pt1current.x)*(pt1.x-pt1current.x) + (pt1.y-pt1current.y)*(pt1.y-pt1current.y)<64*64) &&
                   ((double)(pt2.x-pt2current.x)*(pt2.x-pt2current.x) + (pt2.y-pt2current.y)*(pt2.y-pt2current.y)<64*64)) {
                    // Merge the two
                    (*current)[0] = ((*current)[0]+(*pos)[0])/2;
                    
                    (*current)[1] = ((*current)[1]+(*pos)[1])/2;
                    
                    (*pos)[0]=0;
                    (*pos)[1]=-100;
                }
            }
        }
    }
}

void findCell(Mat &sudoku, Mat &cells, Mat &nums) {
    Mat sudoku_blur;
    GaussianBlur(sudoku, sudoku_blur, Size(7,7), 0);
    
    //imshow("s",sudoku);
    
    adaptiveThreshold(sudoku_blur, nums, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 5, 2);
    //threshold(sudoku, cells, 200, 255, THRESH_BINARY);
    adaptiveThreshold(sudoku, cells, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 11, 2);
    
    //imshow("b",cells);
    //cvWaitKey();
    
    Mat kernel1 = (Mat_<uchar>(3,3) << 0,1,0,1,1,1,0,1,0);
    Mat kernel2 = (Mat_<uchar>(5,5) << 0,0,1,0,0, 0,1,1,1,0, 1,1,1,1,1, 0,1,1,1,0, 0,0,1,0,0);
    Point maxPt = paintBorder(cells);
    //imshow("temp", nums);
    
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

void adjSize (Mat &input, Mat &rst, int std_sz) {
    double im_area =  input.size().height * input.size().width;
    
    if (im_area > std_sz) return;
    
    double sz_ratio = sqrt(std_sz/im_area);
    
    Size new_sz(input.size().width*sz_ratio, input.size().height*sz_ratio);
    
    resize(input, rst, new_sz, INTER_NEAREST);
}

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
    
    if (num_found < 81) cout << "now what?" << endl;
    cell_size /= num_found;
    
    sort(cell_list.begin(), cell_list.end(), compRecty);
    
    for (int i = 0; i < 81; i += 9)
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
    Mat curr_im;
    
    for (int i = 0; i < height; ++i) {
        uchar *p = nums.ptr<uchar>(i);
        for (int j = 0; j < width; ++j) {
            if (static_cast<int>(p[j]) == 127) {
                
                dim_floodFill(nums, Point(j,i), 255, curr_dim, height, width);
                
                curr_area = curr_dim.height*curr_dim.width;
                curr_ratio = (double)curr_dim.height/(double)curr_dim.width;
                
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
                            curr_im = nums(curr_dim);
                            board_im[k] = nums(curr_dim);
                            board[k] = 0;
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
        if (pt.x < max_width-1) {
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
        if (pt.y < max_height-1) {
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
    
    dim.x = left;
    dim.y = top;
    dim.height = bot - top;
    dim.width = right - left;
    
    return area;
}

//void tessPreprocess(Mat &in, Mat &out) {
//    int std_height =
//}

bool compRecty(Rect i, Rect j) {return i.y < j.y;}

bool compRectx(Rect i, Rect j) {return i.x < j.x;}
