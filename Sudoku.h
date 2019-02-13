#ifndef SUDOKU_H
#define SUDOKU_H
//
//  sudoku.h
//  sudoku_solver
//
//  Created by Yuhang Ming on 2019/1/29.
//  Copyright © 2019 Yuhang Ming. All rights reserved.
//

#include <iostream>
#include <vector>

class Sudoku
{
public:
    Sudoku(std::vector< std::vector<int> > digits);
    void Solver();

    std::vector< std::vector<int> > input_digits;
    std::vector< std::vector<int> > result_digits;

private:
    bool HasDuplicate(int row, int col, int num);
    inline bool isEmpty(int row, int col);

    int ele_row, ele_col, ele_box;
    int row, col;
    int tmp_num;
	
};

#endif