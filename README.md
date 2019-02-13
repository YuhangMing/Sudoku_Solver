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
