//
//  sudokuSolver.hpp
//  testCV2
//
//  Created by Trojan Horse on 9/04/2016.
//  Copyright © 2016 NotYourBusiness. All rights reserved.
//

#ifndef sudokuSolver_h
#define sudokuSolver_h

#define UNASSIGNED 0
#define N 9

#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>

using namespace std;

bool solve(int grid[N][N]);

inline bool findUnassigned(int grid[N][N], int &row, int &col);

inline bool isSafe(int grid[N][N], int row, int col, int num);

inline bool inRow(int grid[N][N], int row, int num);

inline bool inCol(int grid[N][N], int col, int num);

inline bool inBox(int grid[N][N], int boxStartRow, int boxStartCol, int num);

inline bool isSafe(int grid[N][N], int row, int col, int num);

void printGrid(int grid[N][N]);

#endif /* sudokuSolver_h */
