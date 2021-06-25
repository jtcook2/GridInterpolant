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
#include <cmath>
#include <random>
#include <algorithm>

#include "GridInterpolant.hpp"

int main(){
  // Same input space and function as README
  std::vector<double> x = {0, 1, 2, 3, 4, 5};
  std::vector<double> y = {6, 7, 8, 9, 10};
  std::vector<std::vector<double>> inputGrid2D = {x, y};
  std::vector<std::vector<double>> inputMesh2D = GridInterpolant::meshgrid(inputGrid2D);

  std::vector<double> X = inputMesh2D.at(0);
  std::vector<double> Y = inputMesh2D.at(1);
  std::vector<double> outputValues = {};
  for(unsigned int ii = 0; ii < X.size(); ii++){
    outputValues.push_back( -6*X.at(ii) + 3*Y.at(ii) );
    std::cout << outputValues.back() << std::endl;
  }

  GridInterpolant gridInterpolant(inputGrid2D, outputValues);

  std::random_device rd; // object used to obtain a seed for the random number generator
  std::mt19937_64 gen(rd()); // engine seeded with rd()
  std::uniform_real_distribution<double> Xdist(x.front(), x.back());
  std::uniform_real_distribution<double> Ydist(y.front(), y.back());
  unsigned int numberOfTestPoints = 1000;
  std::vector<double> outputTest;
  double outputActual;
  std::vector<double> Xtest = {};
  std::vector<double> Ytest = {};
  for(unsigned int testPoint = 0; testPoint < numberOfTestPoints; testPoint++){
    Xtest.push_back(Xdist(gen));
    Ytest.push_back(Ydist(gen));
    outputActual = -6*Xtest.back() + 3*Ytest.back();
    outputTest = gridInterpolant.eval({Xtest.back(), Ytest.back()});
    std::cout << "Xtest = " << Xtest.back() << "  Ytest = " << Ytest.back() << " Actual = " << outputActual 
    << " Interpolant = " << outputTest.at(0) <<  "  Difference = " << abs(outputTest.at(0) - outputActual) << std::endl;
  }


  // New test of 4 dimensional input space
  std::vector<double> w = {-4, -3, -2, -1, 0};
  std::vector<double> z = {11, 12, 13, 14, 15};
  
  std::cout << "Main program executed succesfully." << std::endl;
  return 0;
}
