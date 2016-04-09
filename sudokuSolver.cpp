#include "sudokuSolver.hpp"

bool solve(int grid[N][N]) {
    int row, col;
    if (!findUnassigned(grid, row, col)) 
        return true;
    for (int num = 1; num < 10; ++num) {
        if (isSafe(grid, row, col, num)) {
           grid[row][col] = num;
           if (solve(grid)) 
               return true;
           grid[row][col] = UNASSIGNED;
        }
    }
    return false;
}

inline bool findUnassigned(int grid[N][N], int &row, int &col) {
    for (row = 0; row < N; ++row) 
        for (col = 0; col < N; ++col)
            if (grid[row][col] == UNASSIGNED)
                return true;
    return false;
}

inline bool inRow(int grid[N][N], int row, int num) {
    for (int col = 0; col < N; ++col) 
        if (grid[row][col] == num)
            return true;
    return false;
}

inline bool inCol(int grid[N][N], int col, int num) {
    for (int row = 0; row < N; ++row)
        if (grid[row][col] == num)
            return true;
    return false;
}


inline bool inBox(int grid[N][N], int boxStartRow, int boxStartCol, int num) {
    for (int row = 0; row < 3; ++row)
        for (int col = 0; col < 3; ++col)
            if (grid[row+boxStartRow][col+boxStartCol] == num)
                return true;
    return false;
}

inline bool isSafe(int grid[N][N], int row, int col, int num) {
    return !inRow(grid, row, num) && !inCol(grid, col, num) && !inBox(grid, row - row%3, col - col%3, num);
}

inline void printGrid(int grid[N][N]) {
    for (int row = 0; row < N; ++row) {
        for (int col = 0; col < N; ++col) 
            cout << grid[row][col] << " ";
        cout << endl;
    }
}
