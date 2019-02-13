# Sudoku_Solver

Aiming to write a program that can recognize Sudoku puzzles from an input image and solve the puzzle. 
In order to recognize, I used the neural nets from "neural nets" repository

Progress:

The program can solve any puzzles but having problem reconizing the puzzle images.

compile:

g++ main.cpp Sudoku.cpp -o solver \`pkg-config --cflags --libs opencv\` -I/usr/include/python2.7/ -lpython2.7

run:

./solver (if running on template puzzle)

./solver recognize (if running on image puzzles)


# Sample Output with template puzzle

Loading the puzzle took 0.050137 seconds.

------------------------------------

The input puzzle is: 

0  2  0  0  0  0  0  0  0  

0  0  0  6  0  0  0  0  3  

0  7  4  0  8  0  0  0  0  

0  0  0  0  0  3  0  0  2  

0  8  0  0  4  0  0  1  0  

6  0  0  5  0  0  0  0  0  

0  0  0  0  1  0  7  8  0  

5  0  0  0  0  9  0  0  0  

0  0  0  0  0  0  0  4  0  

Solving the puzzle took 0.081976 seconds.

------------------------------------

The final result of this puzzle is: 

1  2  6  4  3  7  9  5  8  

8  9  5  6  2  1  4  7  3  

3  7  4  9  8  5  1  2  6  

4  5  7  1  9  3  8  6  2  

9  8  3  2  4  6  5  1  7  

6  1  2  5  7  8  3  9  4  

2  6  9  3  1  4  7  8  5  

5  4  8  7  6  9  2  3  1  

7  3  1  8  5  2  6  4  9  

------------------------------------

