//
//  sudoku.cpp
//  sudoku_solver
//
//  Created by Yuhang Ming on 2019/1/29.
//  Copyright © 2019 Yuhang Ming. All rights reserved.
//

#include <iostream>
#include <unistd.h>
#include "Sudoku.h"

// #define REALTIME
// #define DEBUG

Sudoku::Sudoku(std::vector< std::vector<int> > digits) : input_digits(digits), result_digits(digits){}

void Sudoku::Solver(){

    bool isSolved = false;

    // loop through the 9x9 puzzle blocks
    for(int i=0; i<81; i++)
    {
#ifdef DEBUG
        std::cout << std::endl << "i = " << i << ": " << std::endl;
        // usleep(1000000); // in microseconds
#endif
        // extract row and col from i //
        row = i / 9;
        col = i % 9;

        // continue if current value is a given //
        if(not isEmpty(row, col))
        {
#ifdef DEBUG
            std::cout << "---- GIVEN ----" << std::endl;
            std::cout << std::endl;
#endif
            continue;
        }

        // loop through all candidates, store if it is safe //
        for(int num=1; num<10; num++)
        {
            if(not HasDuplicate(row, col, num)){
                result_digits[row][col] = num;
                break;
            }
        }

        // if no element is stored, mistake is made in the previous one //
        if(result_digits[row][col] == 0)
        {
#ifdef DEBUG
            std::cout << "==== Mistakes made, start back tracing ====" << std::endl;
#endif
            while(true)
            {
                // trace one element back, get new row & col indices
                i--;
                row = i/9;
                col = i%9;
#ifdef DEBUG
                std::cout << "new i = " << i << ", row, col = " << row << ", " << col << std::endl;
#endif
                // skip if it is a given
                if(not isEmpty(row, col))
                    continue;
                // increment 1 to the current value until no duplicates found
                result_digits[row][col]++;
                while(HasDuplicate(row, col, result_digits[row][col])){
                    result_digits[row][col]++;
                }
                // set to 0 if the value overflows
                if(result_digits[row][col] > 9){
#ifdef DEBUG
                    std::cout << "==== overflows, set current to 0 and trace one more back ====" << std::endl;
#endif
                    result_digits[row][col] = 0;
                    continue;
                }
                // break the loop if no ill conditions are met
                break;
            }
#ifdef DEBUG
            std::cout << "==== i decreases to " << i << " ====" << std::endl;
#endif
        } // end if backtracing

#ifdef DEBUG
        for(int i=0; i<9; i++){
            for(int j=0; j<9; j++){
                std::cout << result_digits[i][j] << "  ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
#endif

    } // end for 0:81

} // end Sudoku::solver()

bool Sudoku::HasDuplicate(int row, int col, int num){
    // check which box current element belongs to
    int start_row = int(row/3) * 3;
    int start_col = int(col/3) * 3;

// #ifdef DEBUG
//     std::cout << "#### Checking duplicates for num=" << num << std::endl;
//     std::cout << " start row, start col = "  << start_row << ", " << start_col << std::endl;
// #endif
    // do Linear Search for row, col, and 3x3 box
    for (int i=0; i<9; i++){
        // note that ingnore the current one

        // check row
        if(i == col){
            ele_row = 0;
        } else {
            ele_row = result_digits[row][i];
        }
        
        // check col
        if(i == row){
            ele_col = 0;
        } else {
            ele_col = result_digits[i][col];
        }

        // check 3x3 box
        if (i == (3*(row%3)+(col%3)) ) {
            ele_box = 0;
        } else {
            ele_box = result_digits[start_row + i/3][start_col + i%3];
        }
        

        if(ele_row == num || ele_col == num || ele_box == num){
// #ifdef DEBUG
//             std::cout << "#### ele_row, ele_col, ele_box = " << ele_row << ", " << ele_col << ", " << ele_box << std::endl;
//             std::cout << "#### Returning DUPLICATES FOUND" << std::endl;
// #endif
            return true;
        }

    }

    return false;
}


inline bool Sudoku::isEmpty(int row, int col){
    return input_digits[row][col] == 0;
}