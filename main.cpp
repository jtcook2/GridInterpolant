// Simple example test program to get started
#include <iostream>
#include "GridInterpolator.hpp"

using namespace std;

int main(){
  vector<vector<double>> inputGrid = {{1,2}};
  vector<double> outputValues = {1,2};
  

  GridInterpolator gridInterpolator(inputGrid, outputValues);

  vector<double> inputVal = {1.3234};
  vector<double> outputVal = gridInterpolator.eval(inputVal);
  
  cout << "Main program executed succesfully. \n";

  std::cout << "Print outputVal = { ";
  for(auto & val : outputVal){
    std::cout << val << " ";
  }
  std::cout << "} \n";
  
  return 0;
}
