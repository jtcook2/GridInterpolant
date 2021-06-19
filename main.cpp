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
  // Some random input space
  std::vector<double> w = {-4, -3, -2, -1, 0};
  std::vector<double> x = {0, 1, 2, 3, 4, 5};
  std::vector<double> y = {6, 7, 8, 9, 10};
  std::vector<double> z = {11, 12, 13, 14, 15};

  std::vector<std::vector<double>> inputGrid = {x, y};
  std::vector<std::vector<double>> inputMesh = GridInterpolant::meshgrid(inputGrid);

  std::vector<double> outputValues = {18, 19, 22, 27, 34, 43, 21, 22, 25,
                                      30, 37, 46, 24, 25, 28, 33, 40, 49,
                                      27, 28, 31, 36, 43, 52, 30, 31, 34,
                                      39, 46, 55};
                                    
  for(unsigned int ii = 0; ii < outputValues.size(); ii++){
    std::cout << inputMesh.at(0).at(ii) << "  " << inputMesh.at(1).at(ii) << std::endl;
  }

  // Test evaluation
  GridInterpolant gridInterpolant(inputGrid, outputValues);
  std::vector<double> outputVal = gridInterpolant.eval({0.5, 9.9});
  
  
  // Compare output of meshgrid with MATLAB's ndgrid for greater input space
  std::vector<std::vector<double>> inputGridWXYZ = {w, x, y, z};
  std::vector<std::vector<double>> inputMeshWXYZ = GridInterpolant::meshgrid(inputGridWXYZ);
  for(unsigned int ii = 0; ii < w.size()*x.size()*y.size()*z.size(); ii++){
    std::cout << inputMeshWXYZ.at(0).at(ii) << "  " << inputMeshWXYZ.at(1).at(ii) 
    << "  " << inputMeshWXYZ.at(2).at(ii) << "  " << inputMeshWXYZ.at(3).at(ii) << std::endl;
  }

  std::cout << "Main program executed succesfully. \n";
  return 0;
}
