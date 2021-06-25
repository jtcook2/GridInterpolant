/**
 * @file main.cpp
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
#include <cassert>  

#include "GridInterpolant.hpp"

int main(){
  // RNG objects for testing objects work across the input space
  std::random_device rd; // object used to obtain a seed for the random number generator
  std::mt19937_64 gen(rd()); // engine seeded with rd()
  unsigned int numberOfTestPoints = 1000;

  // Other testing variables
  std::vector<double> outputTest;
  double outputActual;
  double Wtest;
  double Xtest;
  double Ytest;
  double Ztest;

  // input grid variables
  std::vector<double> W;
  std::vector<double> X;
  std::vector<double> Y;
  std::vector<double> Z;

  // table output
  std::vector<double> outputValues;

  // ---------------------------------------------------------------------------------
  // Same input space and function as README
  // ---------------------------------------------------------------------------------
  std::vector<double> x = {0, 1, 2, 3, 4, 5};
  std::vector<double> y = {6, 7, 8, 9, 10};
  std::vector<std::vector<double>> inputGrid2D = {x, y};
  std::vector<std::vector<double>> inputMesh2D = GridInterpolant::meshgrid(inputGrid2D);

  X = inputMesh2D.at(0);
  Y = inputMesh2D.at(1);
  outputValues = {};
  for(unsigned int ii = 0; ii < X.size(); ii++){
    outputValues.push_back( -6*X.at(ii) + 3*Y.at(ii) );
  }
  GridInterpolant gridInterpolant2D(inputGrid2D, outputValues);

  std::uniform_real_distribution<double> Xdist(x.front(), x.back());
  std::uniform_real_distribution<double> Ydist(y.front(), y.back());
  
  for(unsigned int testPoint = 0; testPoint < numberOfTestPoints; testPoint++){
    Xtest = Xdist(gen);
    Ytest = Ydist(gen);
    outputActual = -6*Xtest + 3*Ytest;
    outputTest = gridInterpolant2D.eval({Xtest, Ytest});
    assert(abs(outputActual - outputTest.at(0)) < 1e-15);
  }

  // ---------------------------------------------------------------------------------
  // New test of 4 dimensional input space
  // ---------------------------------------------------------------------------------
  std::vector<double> w = {-4, -3, -2, -1, 0};
  std::vector<double> z = {11, 12, 13, 14, 15};

  std::vector<std::vector<double>> inputGrid4D = {w, x, y, z};
  std::vector<std::vector<double>> inputMesh4D = GridInterpolant::meshgrid(inputGrid4D);
 
  W = inputMesh4D.at(0);
  X = inputMesh4D.at(1);
  Y = inputMesh4D.at(2);
  Z = inputMesh4D.at(3);
  outputValues = {};
  for(unsigned int ii = 0; ii < W.size(); ii++){
    outputValues.push_back( 1.55*W.at(ii) - 6*X.at(ii) + 3*Y.at(ii) - 16.33*Z.at(ii) );
  }
  GridInterpolant gridInterpolant4D(inputGrid4D, outputValues);

  std::uniform_real_distribution<double> Wdist(w.front(), w.back());
  std::uniform_real_distribution<double> Zdist(z.front(), z.back());

  for(unsigned int testPoint = 0; testPoint < numberOfTestPoints; testPoint++){
    Wtest = Wdist(gen);
    Xtest = Xdist(gen);
    Ytest = Ydist(gen);
    Ztest = Zdist(gen);
    outputActual = 1.55*Wtest - 6*Xtest + 3*Ytest - 16.33*Ztest;
    outputTest = gridInterpolant4D.eval({Wtest, Xtest, Ytest, Ztest});
    assert(abs(outputActual - outputTest.at(0)) < 1e-15);
  }
  
  std::cout << "Main program executed succesfully." << std::endl;
  return 0;
}
