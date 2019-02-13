//
//  main.cpp
//  sudoku_solver
//
//  Created by Yuhang Ming on 2019/1/29.
//  Copyright © 2019 Yuhang Ming. All rights reserved.
//

#include <Python.h>
#include <numpy/arrayobject.h>

#include <iostream>
#include <vector>
#include <ctime>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include "Sudoku.h"

#define VERBOSE
#define DISPLAY
// #define DEBUG

std::vector< std::vector<int> > read_digits(cv::Mat puzzle, bool recognize_flag);

int main(int argc, const char * argv[]) {

	clock_t time_req;
	time_req = clock();
    
    //// Load the puzzle
    cv::Mat puzzle = cv::imread("easy.png", 0);
#ifdef DISPLAY
    //display image
    cv::namedWindow("Input Puzzle", CV_WINDOW_NORMAL|CV_WINDOW_KEEPRATIO);
    cv::resizeWindow("Input Puzzle", 500, 500);
    cv::imshow("Input Puzzle", puzzle);
    cv::waitKey(0);
#endif
    // read digits from the puzzle image, 0 stands for empty space
    std::vector< std::vector<int> > digits;
    std::cout << argc << std::endl;
    digits = read_digits(puzzle, argc>1);
    time_req = clock() - time_req;
    std::cout << "Loading the puzzle took " << (float)time_req/CLOCKS_PER_SEC << " seconds." << std::endl;


    //// search for results
    Sudoku *new_puzzle = new Sudoku(digits);
    std::cout << "------------------------------------" << std::endl;
    std::cout << "The input puzzle is: " << std::endl;
    for(int i=0; i<9; i++){
    	for(int j=0; j<9; j++){
    		std::cout << new_puzzle->input_digits[i][j] << "  ";
    	}
    	std::cout << std::endl;
    }
    time_req = clock();
    new_puzzle->Solver();
    time_req = clock() - time_req;
    std::cout << "Solving the puzzle took " << (float)time_req/CLOCKS_PER_SEC << " seconds." << std::endl;


    //// print result
    std::cout << std::endl << "------------------------------------" << std::endl;
    std::cout << "The final result of this puzzle is: " << std::endl;
    for(int i=0; i<9; i++){
    	for(int j=0; j<9; j++){
    		std::cout << new_puzzle->result_digits[i][j] << "  ";
    	}
    	std::cout << std::endl;
    }
    std::cout << "------------------------------------" << std::endl;

    return 0;
}


/////// RECOGNIZING FUNCTIONALITY ///////
std::vector< std::vector<int> > read_digits(cv::Mat input, bool recognize_flag){
	std::vector< std::vector<int> > digits;
	cv::Mat puzzle;
	cv::resize(input, puzzle, cv::Size(450, 450));
	int height = puzzle.rows;
	int width = puzzle.cols;
	// int chan = puzzle.channels();
	int depth = puzzle.depth();
	int step = height/9;
	// std::cout << height << ", " << width << ", " 
	//           << chan << ", " << depth << std::endl 
	//           << step << std::endl;

    if (not recognize_flag){
    	std::cout << "Using template puzzle" << std::endl;
    	//## tmp input, change back to recognize from image later ##//
		int grid[9][9] = { { 0, 2, 0, 0, 0, 0, 0, 0, 0 },
						   { 0, 0, 0, 6, 0, 0, 0, 0, 3 },
					       { 0, 7, 4, 0, 8, 0, 0, 0, 0 },
						   { 0, 0, 0, 0, 0, 3, 0, 0, 2 },
						   { 0, 8, 0, 0, 4, 0, 0, 1, 0 },
						   { 6, 0, 0, 5, 0, 0, 0, 0, 0 },
						   { 0, 0, 0, 0, 1, 0, 7, 8, 0 },
						   { 5, 0, 0, 0, 0, 9, 0, 0, 0 },
						   { 0, 0, 0, 0, 0, 0, 0, 4, 0 } };
		for(int i=0; i<9; i++){
	    	std::vector<int> tmp;
	    	for (int j=0; j<9; j++){
	    	// tmp loading
	            tmp.push_back(grid[i][j]);
	    	}
	    	for (int i=0; i<tmp.size(); i++){
	    		std::cout << tmp[i] << ", ";
	    	}
	    	std::cout << std::endl;
	    	digits.push_back(tmp);
	    }
    }
	else{
		std::cout << "Loading puzzle from the image" << std::endl;
		// PyObject
	    PyObject *pPyName, *pPyModule, *pPyArgs, *pPyFunc, *pPyValue;
		PyObject *pPyImage;
		// Initialize python interpreter
	    Py_Initialize();
	    // Import numpy array module
	    _import_array();
	    if(PyArray_API == NULL)
	    {
	        PyErr_Print();
	        fprintf(stderr, "!!!!! Array API not imported...\n");
	        exit(-1);
	    }
	    // Set module path by running some simple python lines
	    PyRun_SimpleString("import sys");
		PyRun_SimpleString("sys.path.insert(0, '/home/yohann/sudoku_solver/')");
		// Build name object
	    pPyName = PyUnicode_FromString("detector");
	    pPyModule = PyImport_Import(pPyName);
	    Py_DECREF(pPyName);
	    if (pPyModule == NULL){
	    	PyErr_Print();
	        fprintf(stderr, "Failed to load \"detector\"\n");
	        exit(-1);
	    }
	 //    pPyFunc = PyObject_GetAttrString(pPyModule, "initialize_nn");
		// if (pPyFunc && PyCallable_Check(pPyFunc))
	 //    {
	 //        pPyArgs = PyTuple_New(0);
	 //        // execute the function
	 //        PyObject_CallObject(pPyFunc, pPyArgs);
		// }
	    
	    std::vector<cv::Mat> blocks;
	    cv::Mat one_block;
	    for(int i=0; i<9; i++){
	    	std::vector<int> tmp;
	    	for (int j=0; j<9; j++){
	    		// crop out thr roi
	    		cv::resize(puzzle(cv::Range(i*step+4, (i+1)*step-4), cv::Range(j*step+4, (j+1)*step-4)), 
	    			one_block, cv::Size(28, 28));
	    		height = one_block.rows;
				width = one_block.cols;
				depth = one_block.depth();
				// std::cout << height << ", " << width << ", " 
				//           << depth << std::endl;

	#ifdef DEBUG
			    std::cout << "Converting cv::Mat to numpy.ndarray\n";
	#endif
	            // convert cv::Mat to numpy.ndarray
			    npy_intp dimensions[2] = {28, 28};
	            // pPyImage = PyArray_SimpleNewFromData(2, dimensions, NPY_UINT8, one_block.data);
	            pPyImage = PyArray_SimpleNewFromData(2, (npy_intp*)&dimensions[0], NPY_UINT8, (void *)one_block.data);

			    // Retrieve the detection function
			    pPyFunc = PyObject_GetAttrString(pPyModule, "detect");
			    if (pPyFunc && PyCallable_Check(pPyFunc))
			    {
			        pPyArgs = PyTuple_New(1);
		            PyTuple_SetItem(pPyArgs, 0, pPyImage);
	#ifdef DEBUG
				    std::cout << "Executing python function\n";
	#endif
			        // execute the function
			        pPyValue = PyObject_CallObject(pPyFunc, pPyArgs);
			        // if (pPyValue != NULL) {
			        //     // printf("#### Result of call: %ld\n\n", PyLong_AsLong(pPyValue));
			        //     // Py_DECREF(pPyValue);
			        // }
			        if(pPyValue == NULL) {
		                PyErr_Print();
		                fprintf(stderr,"Call failed\n");
		                exit(-1);
			        }
			    }
			    else 
			    {
			        if (PyErr_Occurred())
			            PyErr_Print();
			        fprintf(stderr, "Cannot find function \"detect()\"\n");
			        exit(-1);
			    }
	// #ifdef DEBUG
	// 			std::cout << "Decrement Reference\n";
	// #endif
	// 		    Py_DECREF(pPyValue);
	// 	        Py_DECREF(pPyArgs);
	// 		    Py_DECREF(pPyImage);
	// 		    Py_XDECREF(pPyFunc);
	// 		    Py_XDECREF(pPyModule);
	// #ifdef DEBUG
	// 			std::cout << "Finalizing...\n";
	// #endif
	// 		    Py_Finalize();
			    ///////////////////////////////////////////////////////////////////////////////

	            // tmp loading
	            tmp.push_back(PyLong_AsLong(pPyValue));
	    	}
	    	for (int i=0; i<tmp.size(); i++){
	    		std::cout << tmp[i] << ", ";
	    	}
	    	std::cout << std::endl;
	    	digits.push_back(tmp);
	    }

	#ifdef DEBUG
		std::cout << "Decrement Reference\n";
	#endif
	    Py_DECREF(pPyValue);
	    Py_DECREF(pPyArgs);
	    Py_DECREF(pPyImage);
	    Py_XDECREF(pPyFunc);
	    Py_XDECREF(pPyModule);
	#ifdef DEBUG
		std::cout << "Finalizing...\n";
	#endif
	    Py_Finalize();
	}
	
#ifdef DEBUG
	std::cout << "Returning values...\n";
#endif
    return digits;
}