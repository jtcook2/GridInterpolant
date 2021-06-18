/**
 * @file main.hpp
 * This is a simple example to show how to include the GridInterpolant in a program. 
 * It is a manual implementation of the function and grid that interpolated on in 
 * the README.md
 *
 * @author Josh Cook
 *
**/
#include <iostream>
#include <vector>

#include "GridInterpolant.hpp"

int main(){
  std::vector<std::vector<double>> inputGrid = {{0, 1, 2, 3, 4, 5},
                                                 {6, 7, 8, 9, 10}};
  
  std::vector<double> outputValues = {18, 19, 22, 27, 34, 43, 21, 22, 25,
                                      30, 37, 46, 24, 25, 28, 33, 40, 49,
                                      27, 28, 31, 36, 43, 52, 30, 31, 34,
                                      39, 46, 55};

  GridInterpolant gridInterpolant(inputGrid, outputValues);
  std::vector<double> outputVal = gridInterpolant.eval({0.5, 9.9});
  
  std::cout << "Main program executed succesfully. \n";

  std::cout << "Print outputVal = { ";
  for(auto & val : outputVal){
    std::cout << val << " ";
  }
  std::cout << "} \n";
  
  return 0;
}
